#pragma once

#include "light.h"

class SpotLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        auto diff = _coord - param.outv;
        auto dDotN = diff.GetNormal().Dot(param.n);
        auto distance = diff.Length();
        if (dDotN > 0 && distance <= mPower)
        {
            auto dDotD = std::abs(diff.GetNormal().Dot(mDirect));
            if (dDotD > mInRadius)
            {
                param.outc += mColor * dDotN;
                param.outc.Normal();
            }
            else if (dDotD > mOutRadius)
            {
                auto radius = mInRadius - mOutRadius;
                param.outc += mColor * dDotN * ((dDotD - mOutRadius) / radius);
                param.outc.Normal();
            }
        }
    }

public:
    Vec4 mDirect;
    Color mColor;
    float mPower;
    float mInRadius;
    float mOutRadius;
};
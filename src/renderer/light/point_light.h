#pragma once

#include "light.h"

class PointLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        auto diff = _coord - param.outv;
        auto dot = diff.GetNormal().Dot(param.n);
        auto len = diff.Length();
        if (dot > 0 && len < mPower)
        {
            param.outc += mColor * dot * ((mPower - len) / mPower);
            param.outc.Normal();
        }
    }

public:
    Color mColor;
    float mPower;
};
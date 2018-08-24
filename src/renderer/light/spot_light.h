#pragma once

#include "light.h"

class SpotLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        param.outc = mColor * param.outc * (_coord - param.outv).Normal().Dot(param.n);
    }

public:
    Vec4 mDirect;
    Color mColor;
    float mInRadius;
    float mOutRadius;
};
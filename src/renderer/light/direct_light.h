#pragma once

#include "light.h"

class DirectLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        param.outc = mColor * param.outc * (_coord - param.outv).Normal().Dot(param.n);
    }

public:
    Color mColor;
};
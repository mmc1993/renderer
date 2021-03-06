#pragma once

#include "light.h"
#include "../color.h"

class AmbientLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        param.outc = mColor * param.outc;
    }

public:
    Color mColor;
};
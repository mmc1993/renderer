#pragma once

#include "../../base.h"
#include "../pipeline.h"

class Light {
public:
    Light()
    { }

    virtual ~Light()
    { }

    virtual void FragmentFunc(PipelineParam & param) = 0;
};
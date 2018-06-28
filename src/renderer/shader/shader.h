#pragma once

#include "../../base.h"

#include "../pipeline.h"
#include "../matrix.h"
#include "../color.h"
#include "../vec4.h"
#include "../math.h"

class Shader {
public:
    Shader() { }
    virtual ~Shader() { }
    virtual void VertexFunc(PipelineParam & param)
    {
        param.outv = param.v * param.mvp;
    }

    virtual void FragmentFunc(PipelineParam & param)
    {
        param.outc = param.c;
    }
};


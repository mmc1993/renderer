#pragma once

#include "shader.h"

class RGBShader : public Shader {
    virtual void VertexFunc(PipelineParam & param) override
    {
        param.outv = param.v * param.mvp;
    }

    virtual void FragmentFunc(PipelineParam & param) override
    {
        param.outc = param.c * (param.caemraPos - param.outv).Normal().Dot(param.n);
    }
};
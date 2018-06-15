#pragma once

#include "shader.h"
#include "texture.h"

class RGBShader : public Shader {
    virtual Vec4 VertexFunc(Param & param) override
    {
        return Shader::VertexFunc(param);
    }

    virtual std::uint32_t FragmentFunc(Param & param) override
    {
        return Shader::FragmentFunc(param);
    }
};
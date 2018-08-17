#pragma once

#include "shader/shader.h"

class Material {
public:
    void BindShader(const Shader & shader)
    {
        _shader = shader;
    }

    Shader & GetShader()
    {
        return _shader;
    }

private:
    Shader _shader;
};
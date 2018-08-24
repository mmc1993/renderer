#pragma once

#include "light.h"
#include "../color.h"

class AmbientLight : public Light {
public:
    virtual void FragmentFunc(PipelineParam & param) override
    {
        param.outc = _color * param.outc;
    }

    void SetColor(const Color & color)
    {
        _color = color;
    }

private:
    Color _color;
};
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

    void SetCoord(const Vec4 & coord)
    {
        _coord = coord;
    }

    const Vec4 & GetCoord() const
    {
        return _coord;
    }

protected:
    Vec4 _coord;
};
#pragma once

#include "math.h"
#include "vec4.h"
#include "color.h"

class Vertex {
public:
    float u;
    float v;
    Vec4 pt;
    Vec4 normal;
    Color color;

    Vertex operator -(const Vertex & v)
    {
        Vertex vert;
        vert.u = u - v.u;
        vert.v = u - v.v;
        vert.pt = pt - v.pt;
        color = color - v.color;
        return vert;
    }

    Vertex operator *(float v)
    {
        Vertex vert;
        vert.u = u * v;
        vert.v = u * v;
        vert.pt = pt * v;
        color = color * v;
        return vert;
    }

    Vertex LerpFromY(const Vertex & v, float y)
    {
        return Math::Lerp(*this, v, 1.0f / (v.pt.y - pt.y) * y);
    }
};
#pragma once

#include "math.h"
#include "vec4.h"
#include "color.h"

class Vertex {
public:
    Vec4 pt;
    Color color;
    Vec4 normal;
    float u;
    float v;

    Vertex operator -(const Vertex & v) const
    {
        Vertex vert;
        vert.u = u - v.u;
        vert.v = u - v.v;
        vert.pt = pt - v.pt;
        vert.color = color - v.color;
        return vert;
    }

    Vertex operator +(const Vertex & v) const
    {
        Vertex vert;
        vert.u = u + v.u;
        vert.v = u + v.v;
        vert.pt = pt + v.pt;
        vert.color = color + v.color;
        return vert;
    }

    Vertex operator *(float v) const
    {
        Vertex vert;
        vert.u = u * v;
        vert.v = u * v;
        vert.pt = pt * v;
        vert.color = color * v;
        return vert;
    }

    Vertex LerpFromY(const Vertex & v, float y) const
    {
        return Math::Lerp(*this, v, 1.0f / (v.pt.y - pt.y) * y);
    }
};
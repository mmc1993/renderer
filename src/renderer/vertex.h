#pragma once

#include "math.h"
#include "vec4.h"
#include "color.h"

class Vertex {
public:
    Vec4 world;
    Vec4 coord;
    Color color;
    Vec4 normal;
    float u;
    float v;

    Vertex operator -(const Vertex & v) const
    {
        Vertex vert = *this;
        vert.u -= v.u;
        vert.v -= v.v;
        vert.coord -= v.coord;
        vert.world -= v.world;
        vert.color -= v.color;
        return vert;
    }

    Vertex operator +(const Vertex & v) const
    {
        Vertex vert = *this;
        vert.u += v.u;
        vert.v += v.v;
        vert.coord += v.coord;
        vert.world += v.world;
        vert.color += v.color;
        return vert;
    }

    Vertex operator *(float v) const
    {
        Vertex vert = *this;
        vert.u = u * v;
        vert.v = u * v;
        vert.coord = coord * v;
        vert.world = world * v;
        vert.color = color * v;
        return vert;
    }

    Vertex LerpFromY(const Vertex & v, float y) const&
    {
        return Math::Lerp(*this, v, 1.0f / (v.coord.y - coord.y) * y);
    }
};
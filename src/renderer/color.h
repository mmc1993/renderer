#pragma once

#include "math.h"
#include <cstdint>

class Color {
public:
    float r, g, b, a;

    Color(float _r, float _g, float _b, float _a)
    {
        r = _r; g = _g; 
        b = _b; a = _a;
    }

    Color(float _r, float _g, float _b)
    {
        r = _r; g = _g;
        b = _b; a = 1.0f;
    }

    Color(const Color & v)
    {
        *this = v;
    }

    Color()
    {
        r = g = b = a = 0;
    }

    Color & operator =(const Color & v)
    {
        r = v.r; 
        g = v.g; 
        b = v.b; 
        a = v.a;
        return *this;
    }

    Color operator +(const Color & v) const
    {
        return Color(r + v.r, g + v.g, b + v.b, a + v.a);
    }

    Color operator -(const Color & v) const
    {
        return Color(r - v.r, g - v.g, b - v.b, a - v.a);
    }

    Color operator *(const Color & v) const
    {
        return Color(r * v.r, g * v.g, b * v.b, a * v.a);
    }

    Color operator *(float v) const
    {
        return Color(r * v, g * v, b * v, a * v);
    }

    Color operator /(const Color & v) const
    {
        return Color(r / v.r, g / v.g, b / v.b, a / v.a);
    }

    Color operator /(float v) const
    {
        return Color(r / v, g / v, b / v, a / v);
    }

    std::uint32_t ToRGBA() const
    {
        return (Color::F2D(r) << 24) |
               (Color::F2D(g) << 16) |
               (Color::F2D(b) << 8) |
               (Color::F2D(a));
    }

    void Normal()
    {
        r = Math::Clamp(r, 1.0f, 0.0f);
        g = Math::Clamp(g, 1.0f, 0.0f);
        b = Math::Clamp(b, 1.0f, 0.0f);
        a = Math::Clamp(a, 1.0f, 0.0f);
    }

    inline static std::uint8_t F2D(float f)
    {
        return static_cast<std::uint8_t>(f * 255.0f);
    }
};
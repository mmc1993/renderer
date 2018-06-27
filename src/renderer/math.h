#pragma once

class Math {
public:
    template <class T>
    static T Lerp(const T & v1, const T & v2, float t)
    {
        return (v2 - v1) * t + v1;
    }

    template <class T>
    static T Clamp(const T & v, const T & max, const T & min)
    {
        return v > max ? max : v < min ? min : v;
    }
};

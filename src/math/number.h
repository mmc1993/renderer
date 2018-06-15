#pragma once

#include <type_traits>

class Number {
public:
    template <class T>
    static T Lerp(const T & v1, const T & v2, float t)
    {
        return (v2 - v1) * t + v1;
    }

    template <class T>
    static T Clamp(const T & v, const T & max, const T & min)
    {
        static_assert(std::is_arithmetic<T>::value);
        return v > max ? max : v < min ? min : v;
    }
};

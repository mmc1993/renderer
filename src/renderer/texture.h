#pragma once

#include "../base.h"
#include "color.h"
#include "math.h"

class Texture {
public:
    Texture(): _data(nullptr), _w(0), _h(0), _channel(0)
    { }
    
    ~Texture();

    Color GetColor(float u, float v)
    {
        assert(_data != nullptr);
        auto s = 1.0f / 255.0f;
        auto i = ConverToIndex(u, v);
        return { static_cast<float>(_data[i    ]) * s, 
                 static_cast<float>(_data[i + 1]) * s,
                 static_cast<float>(_data[i + 2]) * s };
    }

    std::uint32_t GetW() const { return _w; }
    std::uint32_t GetH() const { return _h; }
    bool InitFromFile(const std::string & fname);

private:
    size_t ConverToIndex(float u, float v)
    {
        u = Math::Clamp(u, 1.0f, 0.0f);
        v = Math::Clamp(v, 1.0f, 0.0f);
        auto x = static_cast<std::uint32_t>(u * (_w - 1));
        auto y = static_cast<std::uint32_t>(v * (_h - 1));
        return static_cast<size_t>(y * _w * _channel + x * _channel);
    }

private:
    std::uint32_t _channel;
    std::uint32_t _w;
    std::uint32_t _h;
    std::uint8_t * _data;
};

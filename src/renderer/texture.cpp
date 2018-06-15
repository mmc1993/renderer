#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../third/stb_image.h"

Texture::~Texture()
{
    if (_data != nullptr)
    {
        stbi_image_free(_data);
        _data = nullptr;
    }
}

bool Texture::InitFromFile(const std::string & fname)
{
    assert(_data == nullptr);
    int w = 0, h = 0, channel = 0;
    _data = stbi_load(fname.c_str(), &w, &h, &channel, 0);
    if (_data != nullptr)
    {
        _w = static_cast<std::uint32_t>(w);
        _h = static_cast<std::uint32_t>(h);
        _channel = static_cast<std::uint32_t>(channel);
    }
    return _data != nullptr;
}

#pragma once

#include "base_cache.h"
#include "../renderer/shader/shader.h"

class ShaderCache: public BaseCache<Shader> {
public:
    template <class T>
    bool Load(const std::string & key)
    {
        if (nullptr == Get(key))
        {
            Add(key, T());
        }
        return true;
    }
};
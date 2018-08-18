#pragma once

#include "base_cache.h"
#include "../renderer/material.h"

class MaterialCache: public BaseCache<Material> {
public:
    bool Bind(const std::string & fname, const Material & material)
    {
        if (nullptr == Get(fname))
        {
            Add(fname, material);
        }
        return true;
    }
};
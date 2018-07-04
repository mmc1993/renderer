#pragma once

#include "../base.h"

template <class T>
class BaseCache {
public:
    T * Get(const std::string & key)
    {
        auto it = _cache.find(key);
        if (it != _cache.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    void Del(const std::string & key)
    {
        _cache.erase(key);
    }

protected:
    void Add(const std::string & key, const T & val)
    {
        _cache.insert({ key, val });
    }

private:
    std::unordered_map<std::string, T> _cache;
};
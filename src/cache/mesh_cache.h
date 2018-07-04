#pragma once

#include "base_cache.h"
#include "../renderer/mesh.h"
#include "../renderer/io/file/file.h"

class MeshCache: public BaseCache<Mesh> {
public:
    bool Load(const std::string & fname)
    {
        if (nullptr == Get(fname))
        {
            Mesh mesh;
            if (File::LoadMesh(fname, &mesh))
            {
                Add(fname, mesh);
                return true;
            }
            return false;
        }
        return true;
    }

private:
    std::unordered_map<std::string, Mesh> _cache;
};
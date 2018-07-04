#pragma once

#include "../base.h"
#include "vertex.h"

class Mesh {
public:
    void AddVertexs(const Vertex & v0, const Vertex & v1, const Vertex & v2)
    {
        _mesh.push_back(v0);
        _mesh.push_back(v1);
        _mesh.push_back(v2);
    }
    
    void Update(size_t index, const Vertex & vert)
    {
        _mesh.at(index) = vert;
    }

    const std::vector<Vertex> & GetMesh() const
    {
        return _mesh;
    }

private:
    std::vector<Vertex> _mesh;
};
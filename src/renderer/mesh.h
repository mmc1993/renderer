#pragma once

#include "../base.h"
#include "vertex.h"

class Mesh {
public:
    void SetMesh(std::vector<Vertex> && mesh)
    {
        _mesh = std::move(mesh);
    }

    void Update(size_t index, const Vertex & vert)
    {
        _mesh.at(index) = vert;
    }

    const std::vector<Vertex> & GetMesh() const
    {
        const_cast<Mesh *>(this)->GetMesh();
    }

    const std::vector<Vertex> & GetMesh()
    {
        return _mesh;
    }

private:
    std::vector<Vertex> _mesh;
};
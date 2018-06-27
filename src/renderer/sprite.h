#pragma once

#include "mesh.h"
#include "material.h"

class Sprite {
public:
    const Mesh & GetMesh() const
    {
        return const_cast<Sprite *>(this)->GetMesh();
    }

    const Material & GetMaterial() const
    {
        return const_cast<Sprite *>(this)->GetMaterial();
    }

    const Mesh & GetMesh()
    {
        return *_pMesh;
    }

    const Material & GetMaterial()
    {
        return *_pMaterial;
    }

    void SetMesh(std::shared_ptr<Mesh> pMesh)
    {
        _pMesh = pMesh;
    }

    void SetMaterial(std::shared_ptr<Material> pMaterial)
    {
        _pMaterial = pMaterial;
    }

private:
    std::shared_ptr<Mesh> _pMesh;

    std::shared_ptr<Material> _pMaterial;
};
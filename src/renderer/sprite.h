#pragma once

#include "matrix.h"
#include "math.h"
#include "vec4.h"
#include "mesh.h"
#include "material.h"

class Sprite {
public:
    struct Transform {
        Transform()
        {
            change = true;
        }
        bool change;
        float sx, sy, sz;
        float px, py, pz;
        float rx, ry, rz;
        Matrix4x4 transform;
    };
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

    void SetCoord(float x, float y, float z);
    void SetRotateX(float x);
    void SetRotateY(float y);
    void SetRotateZ(float z);
    void SetScale(float s);

    virtual void OnDraw();

private:
    Transform _transform;

    std::shared_ptr<Mesh> _pMesh;

    std::shared_ptr<Material> _pMaterial;
};
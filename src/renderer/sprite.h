#pragma once

#include "matrix.h"
#include "math.h"
#include "vec4.h"
#include "mesh.h"
#include "material.h"
#include "renderer.h"

class Sprite {
public:
    struct Transform {
        Transform()
        {
            change = true;
            sx = sy = sz = 1;
            px = py = pz = 0;
            rx = ry = rz = 0;
        }
        bool change;
        float sx, sy, sz;
        float px, py, pz;
        float rx, ry, rz;
        Matrix4x4 transform;
    };
public:
    void AddChild(Sprite * child)
    {
        _childs.push_back(child);
    }

    void DelChild(Sprite * child)
    {
        auto it = std::remove(_childs.begin(), _childs.end(), child);
        if (it != _childs.end())
        {
            _childs.erase(it);
        }
    }

    void SetMesh(Mesh * pMesh)
    {
        _mesh = pMesh;
    }

    void SetMaterial(Material * pMaterial)
    {
        _material = pMaterial;
    }

    void SetCoord(float x, float y, float z)
    {
        _transform.px = x;
        _transform.py = y;
        _transform.pz = z;
        _transform.change = true;
    }

    void SetRotateX(float x)
    {
        _transform.rx = x;
        _transform.change = true;
    }

    void SetRotateY(float y)
    {
        _transform.ry = y;
        _transform.change = true;
    }

    void SetRotateZ(float z)
    {
        _transform.rz = z;
        _transform.change = true;
    }

    void SetRotate(float x, float y, float z)
    {
        _transform.rx = x;
        _transform.ry = y;
        _transform.rz = z;
        _transform.change = true;
    }

    void SetScale(float s)
    {
        _transform.sx = s;
        _transform.sy = s;
        _transform.sz = s;
        _transform.change = true;
    }

    const Transform & GetTransform() const
    {
        return _transform;
    }

    virtual void OnDraw(Renderer * renderer, const Matrix4x4 * parent = nullptr, bool isUpdate = false);

private:

private:
    std::vector<Sprite *> _childs;

    Transform _transform;

    Material * _material;

    Mesh * _mesh;
};
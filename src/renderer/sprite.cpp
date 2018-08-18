#include "sprite.h"

void Sprite::OnDraw(Renderer * renderer, const Matrix4x4 * parent)
{
    if (_transform.change)
    {
        _transform.change = false;
        _transform.transform.Identity();
        _transform.transform.Scale(_transform.sx, _transform.sy, _transform.sz);
        _transform.transform.Rotate(1, 0, 0, D2R(_transform.rx));
        _transform.transform.Rotate(0, 1, 0, D2R(_transform.ry));
        _transform.transform.Rotate(0, 0, 1, D2R(_transform.rz));
        _transform.transform.Translate(_transform.px, _transform.py, _transform.pz);
    }
    Matrix4x4 mat4x4;
    if (parent != nullptr)
    {
        mat4x4 = *parent * _transform.transform;
    }
    else
    {
        mat4x4 = _transform.transform;
    }

    renderer->SetModelMatrix(mat4x4);
    renderer->Primitive(_mesh, _material);

    for (auto child : _childs)
    {
        child->OnDraw(renderer, &mat4x4);
    }
}

#include "sprite.h"

void Sprite::OnDraw(Renderer * renderer, const Matrix4x4 * parent, bool isUpdate)
{
    Matrix4x4 transform;

    if (parent == nullptr)
    {
        transform.Identity();
    }
    else
    {
        transform = *parent;
    }

    if (isUpdate || _transform.change)
    {
        transform.Scale(_transform.sx, _transform.sy, _transform.sz);
        transform.Rotate(1, 0, 0, D2R(_transform.rx));
        transform.Rotate(0, 1, 0, D2R(_transform.ry));
        transform.Rotate(0, 0, 1, D2R(_transform.rz));
        transform.Translate(_transform.px, _transform.py, _transform.pz);
        _transform.transform = transform;
        _transform.change = false;
        isUpdate = true;
    }

    renderer->SetModelMatrix(_transform.transform);

    renderer->Primitive(_mesh, _material);

    for (auto child : _childs)
    {
        child->OnDraw(renderer, &transform, isUpdate);
    }
}

#include "sprite.h"

void Sprite::OnDraw(Renderer * renderer)
{
    if (_transform.change)
    {
        _transform.change = false;
        _transform.transform.Identity();
        _transform.transform.Scale(_transform.sx, _transform.sy, _transform.sz);
        _transform.transform.Rotate(1, 0, 0, _transform.rx);
        _transform.transform.Rotate(0, 1, 0, _transform.ry);
        _transform.transform.Rotate(0, 0, 1, _transform.rz);
        _transform.transform.Translate(_transform.px, _transform.py, _transform.pz);
    }
    renderer->SetModelMatrix(_transform.transform);
    renderer->Primitive(_mesh, _material);
}

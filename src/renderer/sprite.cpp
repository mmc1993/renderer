#include "sprite.h"

void Sprite::SetCoord(float x, float y, float z)
{
    _transform.px = x;
    _transform.py = y;
    _transform.pz = z;
    _transform.change = true;
}

void Sprite::SetRotateX(float x)
{
    _transform.rx = x;
    _transform.change = true;
}

void Sprite::SetRotateY(float y)
{
    _transform.ry = y;
    _transform.change = true;
}

void Sprite::SetRotateZ(float z)
{
    _transform.rz = z;
    _transform.change = true;
}

void Sprite::SetScale(float s)
{
    _transform.sx = s;
    _transform.sy = s;
    _transform.sz = s;
    _transform.change = true;
}

void Sprite::OnDraw()
{
    if (_transform.change)
    {
        _transform.change = false;
        _transform.transform.Identity();
        _transform.transform.Translate(_transform.px, _transform.py, _transform.pz);
        _transform.transform.Rotate(1, 0, 0, _transform.rx);
        _transform.transform.Rotate(0, 1, 0, _transform.ry);
        _transform.transform.Rotate(0, 0, 1, _transform.rz);
        _transform.transform.Scale(_transform.sx, _transform.sy, _transform.sz);
    }
    //  ÷¥––‰÷»æ
}

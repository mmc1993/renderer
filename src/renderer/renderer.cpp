#include "renderer.h"

Renderer::Renderer(): _drawMode(DrawMode::kLINE)
{ }

void Renderer::SetFar(float vfar)
{ 
    _camera.vfar = vfar;
    _transform.project.Identity();
    _transform.project.m[2][3] = 1.0f;
    _transform.project.m[3][0] = _transform.project.m[3][1]
                               = _transform.project.m[3][2]
                               = _transform.project.m[3][3] = 0;
    _transform.mvp = _transform.view * _transform.project;
}

void Renderer::Clear(float r, float g, float b)
{
    auto color = Color::ToRGBA(r, g, b, 1.0f);
    std::fill(_buffer.frame.get(), _buffer.frame.get() + _buffer.GetCount(), color);
    std::fill(_buffer.zorder.get(), _buffer.zorder.get() + _buffer.GetCount(), UINT32_MAX);
}

void Renderer::SetDrawMode(std::uint8_t mode)
{
    _drawMode = mode;
}

void Renderer::SetLineRGB(const std::uint32_t rgb)
{
    _lineRGB = rgb;
}

void Renderer::SetViewPort(std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2)
{
    _viewport.x = x1;
    _viewport.y = y1;
    _viewport.w = x2 - x1;
    _viewport.h = y2 - y1;
}

void Renderer::SetBufferSize(std::uint32_t w, std::uint32_t h)
{
    _bufferWH.w = w;
    _bufferWH.h = h;

    _buffer.Init(w * h);

    _transform.screen.Identity();
    _transform.screen.m[0][0] = w * 0.5f;
    _transform.screen.m[1][1] = h * 0.5f;
    _transform.screen.m[3][0] = w * 0.5f;
    _transform.screen.m[3][1] = h * 0.5f;
}

void Renderer::LookAt(const Vec4 & eye, const Vec4 & up, const Vec4 & at)
{
    auto right = up.Cross(at);
    _transform.view.m[0][0] = right.x;
    _transform.view.m[1][0] = right.y;
    _transform.view.m[2][0] = right.z;
    _transform.view.m[3][0] = eye.x;

    _transform.view.m[0][1] = up.x;
    _transform.view.m[1][1] = up.y;
    _transform.view.m[2][1] = up.z;
    _transform.view.m[3][1] = eye.y;

    _transform.view.m[0][2] = at.x;
    _transform.view.m[1][2] = at.y;
    _transform.view.m[2][2] = at.z;
    _transform.view.m[3][2] = eye.z;

    _transform.view.m[0][3] = 0;
    _transform.view.m[1][3] = 0;
    _transform.view.m[2][3] = 0;
    _transform.view.m[3][3] = 1;
    _transform.mvp = _transform.view * _transform.project;

    _camera.up = up;
    _camera.at = at;
    _camera.eye = eye;
}

void Renderer::Primitive(size_t count, Vertex * vertexs, Shader * shader)
{
    //  初始化着色参数
    _pRefShader = shader;
    for (auto i = 0; i != count; i += 3)
    {
        Primitive(vertexs[i    ],
                  vertexs[i + 1],
                  vertexs[i + 2]);
    }
    _pRefShader = nullptr;
}

void Renderer::Primitive(Vertex vert1, Vertex vert2, Vertex vert3)
{
    if (!CheckBackCut(vert1.pt, vert2.pt, vert3.pt, &vert1.normal))
    {
        return;
    }
    vert2.normal = vert1.normal;
    vert3.normal = vert1.normal;

    _shaderParam.vpMat = _transform.mvp;

    _shaderParam.point = vert1.pt;
    _shaderParam.normal = vert1.normal;
    vert1.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckViewCut(vert1.pt) != 0) { return; }

    _shaderParam.point = vert2.pt;
    _shaderParam.normal = vert2.normal;
    vert2.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckViewCut(vert2.pt) != 0) { return; }

    _shaderParam.point = vert3.pt;
    _shaderParam.normal = vert3.normal;
    vert3.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckViewCut(vert3.pt) != 0) { return; }

    vert1.pt.x /= vert1.pt.w;
    vert1.pt.y /= vert1.pt.w;
    vert1.pt.w /= vert1.pt.w;

    vert2.pt.x /= vert2.pt.w;
    vert2.pt.y /= vert2.pt.w;
    vert2.pt.w /= vert2.pt.w;

    vert3.pt.x /= vert3.pt.w;
    vert3.pt.y /= vert3.pt.w;
    vert3.pt.w /= vert3.pt.w;

    //  变换到平面坐标
    vert1.pt *= _transform.screen;
    vert2.pt *= _transform.screen;
    vert3.pt *= _transform.screen;

    if (_drawMode & DrawMode::kFILL)
    {
        DrawTriangle(vert1, vert2, vert3);
    }

    if (_drawMode & DrawMode::kLINE)
    {
        DrawLine(vert1.pt.x, vert1.pt.y, vert2.pt.x, vert2.pt.y);
        DrawLine(vert2.pt.x, vert2.pt.y, vert3.pt.x, vert3.pt.y);
        DrawLine(vert3.pt.x, vert3.pt.y, vert1.pt.x, vert1.pt.y);
    }
}

void Renderer::DrawTriangle(const Vertex & vert1, const Vertex & vert2, const Vertex & vert3)
{
    const Vertex * pVert[3] = { nullptr, nullptr, nullptr };
    if (vert1.pt.y == vert2.pt.y)
    {
        pVert[0] = &vert3;
        pVert[1] = &vert1;
        pVert[2] = &vert2;
    }
    else if (vert1.pt.y == vert3.pt.y)
    {
        pVert[0] = &vert2;
        pVert[1] = &vert1;
        pVert[2] = &vert3;
    }
    else if (vert2.pt.y == vert3.pt.y)
    {
        pVert[0] = &vert1;
        pVert[1] = &vert2;
        pVert[2] = &vert3;
    }
    else
    {
        Vertex verts[3];
        if (vert1.pt.y > vert2.pt.y && vert1.pt.y > vert3.pt.y)
        {
            verts[0] = vert1;
            verts[1] = vert2.pt.y > vert3.pt.y ? vert2 : vert3;
            verts[2] = vert2.pt.y > vert3.pt.y ? vert3 : vert2;
        }
        else if (vert2.pt.y > vert1.pt.y && vert2.pt.y > vert3.pt.y)
        {
            verts[0] = vert2;
            verts[1] = vert1.pt.y > vert3.pt.y ? vert1 : vert3;
            verts[2] = vert1.pt.y > vert3.pt.y ? vert3 : vert1;
        }
        else if (vert3.pt.y > vert1.pt.y && vert3.pt.y > vert2.pt.y)
        {
            verts[0] = vert3;
            verts[1] = vert1.pt.y > vert2.pt.y ? vert1 : vert2;
            verts[2] = vert1.pt.y > vert2.pt.y ? vert2 : vert1;
        }
        auto vert4 = verts[0].LerpFromY(verts[2], 
                                        verts[1].pt.y - verts[0].pt.y);
        vert4.pt.y = verts[1].pt.y;
        DrawTriangle(verts[0], vert4, verts[1]);
        DrawTriangle(verts[1], vert4, verts[2]);
    }

    if (pVert[0] != nullptr || pVert[1] != nullptr || pVert[2] != nullptr)
    {
        if (pVert[1]->pt.x > pVert[2]->pt.x)
        {
            std::swap(pVert[1], pVert[2]);
        }

        if (pVert[0]->pt.y < pVert[1]->pt.y)
        {
            DrawTriangleTop(pVert);
        }
        else
        {
            DrawTriangleBottom(pVert);
        }
    }
}

void Renderer::DrawTriangleBottom(const Vertex ** pVert)
{
    auto dy = pVert[1]->pt.y - pVert[0]->pt.y;
    for (auto y = 0; y >= dy; --y)
    {
        auto start = pVert[0]->LerpFromY(*pVert[1], (float)y);
        auto end = pVert[0]->LerpFromY(*pVert[2], (float)y);
        DrawScanLine(start, end);
    }
}

void Renderer::DrawTriangleTop(const Vertex ** pVert)
{
    auto dy = pVert[1]->pt.y - pVert[0]->pt.y;
    for (auto y = dy; y >=0; --y)
    {
        auto start = pVert[0]->LerpFromY(*pVert[1], (float)y);
        auto end = pVert[0]->LerpFromY(*pVert[2], (float)y);
        DrawScanLine(start, end);
    }
}

void Renderer::DrawScanLine(const Vertex & start, const Vertex & end)
{
    auto w = std::ceil(end.pt.x - start.pt.x)/* + 0.5f*/;
    auto cs = (end.color - start.color) / w;
    auto us = (end.u - start.u) / w;
    auto vs = (end.v - start.v) / w;
    auto vert = start;
    for (auto x = 0; x <= w; ++x)
    {
        DrawPoint(vert);
        vert.u = vert.u + us;
        vert.v = vert.v + vs;
        vert.pt.x = vert.pt.x + 1.0f;
        vert.color = vert.color + cs;
    }
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
    auto diffx = x2 - x1;
    auto diffy = y2 - y1;
    auto count = size_t(std::max(std::abs(diffx),
                                 std::abs(diffy)));
    auto stepx = diffx / count;
    auto stepy = diffy / count;
    for (auto i = 0; i != count; ++i)
    {
        auto index = _buffer.ToIndex(x1, y1, _bufferWH.w);
        _buffer.frame[index] = _lineRGB;
        x1 += stepx; y1 += stepy;
    }
}

inline void Renderer::DrawPoint(const Vertex & vert)
{
    auto index = _buffer.ToIndex(vert.pt.x, vert.pt.y, _bufferWH.w);
    assert(index < _bufferWH.Product());
    if (vert.pt.z < _buffer.zorder[index])
    {
        _shaderParam.u = vert.u;
        _shaderParam.v = vert.v;
        _shaderParam.point = vert.pt;
        _shaderParam.color = vert.color;
        _buffer.frame[index] = _pRefShader->FragmentFunc(_shaderParam);
        _buffer.zorder[index] = static_cast<std::uint32_t>(vert.pt.z);
    }
}

inline std::uint8_t Renderer::CheckViewCut(const Vec4 & vec)
{
    auto ret = (std::uint8_t)0;
    if (vec.x < -vec.w) ret |= 1;
    if (vec.x > vec.w) ret |= 2;
    if (vec.y < -vec.w) ret |= 4;
    if (vec.y > vec.w) ret |= 8;
    if (vec.z < -vec.w) ret |= 16;
    if (vec.z > vec.w) ret |= 32;
    return ret;
}

inline bool Renderer::CheckBackCut(const Vec4 & pt1, const Vec4 & pt2, const Vec4 & pt3, Vec4 * outNormal)
{
    auto normal = (pt2 - pt1).Cross(pt3 - pt2);
    auto direct = (pt1 - _camera.eye);
    if (normal.Dot(direct) > 0) { return false; }
    *outNormal = normal;
    outNormal->Normal();
    return true;
}
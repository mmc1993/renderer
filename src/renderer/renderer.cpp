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

void Renderer::SetModelMatrix(const Matrix4x4 & mat)
{
    _transform.model = mat;
    _transform.mvp = _transform.model
                   * _transform.view 
                   * _transform.project;
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

void Renderer::Primitive(Mesh * mesh, Material * material)
{
    _render.mesh = mesh;
    _render.material = material;
    _render.param.material = material;
    _render.param.mvp = _transform.mvp;
    const auto & vertexs = mesh->GetMesh();
    for (auto i = 0; i != vertexs.size(); i += 3)
    {
        Primitive(vertexs.at(i    ),
                  vertexs.at(i + 1),
                  vertexs.at(i + 2));
    }
}

void Renderer::Primitive(Vertex v1, Vertex v2, Vertex v3)
{
    //  裁剪
    VertexShader(v1, &v1.coord);
    if (CheckViewCut(v1.coord) != 0) { return; }
    VertexShader(v2, &v2.coord);
    if (CheckViewCut(v2.coord) != 0) { return; }
    VertexShader(v3, &v3.coord);
    if (CheckViewCut(v3.coord) != 0) { return; }

    //  背面剔除
    if (!CheckBackCut(v1.coord, v2.coord, v3.coord, &v1.normal)) { return; }
    v2.normal = v1.normal;
    v3.normal = v1.normal;

    //  这里可以通过延后 / w, 跳过这一步, 但是我想赶紧结束这个项目, 所以将就一下算了.
    //  保存世界坐标
    v1.world = v1.coord;
    v2.world = v2.coord;
    v3.world = v3.coord;

    v1.coord.x /= v1.coord.w; v1.coord.y /= v1.coord.w; v1.coord.w /= v1.coord.w;
    v2.coord.x /= v2.coord.w; v2.coord.y /= v2.coord.w; v2.coord.w /= v2.coord.w;
    v3.coord.x /= v3.coord.w; v3.coord.y /= v3.coord.w; v3.coord.w /= v3.coord.w;

    //  变换到平面坐标
    v1.coord *= _transform.screen;
    v2.coord *= _transform.screen;
    v3.coord *= _transform.screen;

    if (_drawMode & DrawMode::kFILL)
    {
        DrawTriangle(v1, v2, v3);
    }

    if (_drawMode & DrawMode::kLINE)
    {
        DrawLine(v1.coord.x, v1.coord.y, v2.coord.x, v2.coord.y);
        DrawLine(v2.coord.x, v2.coord.y, v3.coord.x, v3.coord.y);
        DrawLine(v3.coord.x, v3.coord.y, v1.coord.x, v1.coord.y);
    }
}

void Renderer::DrawTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3)
{
    const Vertex * pVert[3] = { nullptr, nullptr, nullptr };
    if (v1.coord.y == v2.coord.y)
    {
        pVert[0] = &v3;
        pVert[1] = &v1;
        pVert[2] = &v2;
    }
    else if (v1.coord.y == v3.coord.y)
    {
        pVert[0] = &v2;
        pVert[1] = &v1;
        pVert[2] = &v3;
    }
    else if (v2.coord.y == v3.coord.y)
    {
        pVert[0] = &v1;
        pVert[1] = &v2;
        pVert[2] = &v3;
    }
    else
    {
        Vertex verts[3];
        if (v1.coord.y > v2.coord.y && v1.coord.y > v3.coord.y)
        {
            verts[0] = v1;
            verts[1] = v2.coord.y > v3.coord.y ? v2 : v3;
            verts[2] = v2.coord.y > v3.coord.y ? v3 : v2;
        }
        else if (v2.coord.y > v1.coord.y && v2.coord.y > v3.coord.y)
        {
            verts[0] = v2;
            verts[1] = v1.coord.y > v3.coord.y ? v1 : v3;
            verts[2] = v1.coord.y > v3.coord.y ? v3 : v1;
        }
        else if (v3.coord.y > v1.coord.y && v3.coord.y > v2.coord.y)
        {
            verts[0] = v3;
            verts[1] = v1.coord.y > v2.coord.y ? v1 : v2;
            verts[2] = v1.coord.y > v2.coord.y ? v2 : v1;
        }
        auto vert4 = verts[0].LerpFromY(verts[2], 
                                        verts[1].coord.y - verts[0].coord.y);
        vert4.coord.y = verts[1].coord.y;
        DrawTriangle(verts[0], vert4, verts[1]);
        DrawTriangle(verts[1], vert4, verts[2]);
    }

    if (pVert[0] != nullptr || pVert[1] != nullptr || pVert[2] != nullptr)
    {
        if (pVert[1]->coord.x > pVert[2]->coord.x)
        {
            std::swap(pVert[1], pVert[2]);
        }

        if (pVert[0]->coord.y < pVert[1]->coord.y)
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
    auto dy = pVert[1]->coord.y - pVert[0]->coord.y;
    for (auto y = 0; y >= dy; --y)
    {
        auto start = pVert[0]->LerpFromY(*pVert[1], (float)y);
        auto end = pVert[0]->LerpFromY(*pVert[2], (float)y);
        DrawScanLine(start, end);
    }
}

void Renderer::DrawTriangleTop(const Vertex ** pVert)
{
    auto dy = pVert[1]->coord.y - pVert[0]->coord.y;
    for (auto y = dy; y >=0; --y)
    {
        auto start = pVert[0]->LerpFromY(*pVert[1], (float)y);
        auto end = pVert[0]->LerpFromY(*pVert[2], (float)y);
        DrawScanLine(start, end);
    }
}

void Renderer::DrawScanLine(const Vertex & start, const Vertex & end)
{
    auto w = std::ceil(end.coord.x - start.coord.x);
    auto cs = (end.color - start.color) / w;
    auto ws = (end.world - start.world) / w;
    auto us = (end.u - start.u) / w;
    auto vs = (end.v - start.v) / w;
    auto vert = start;
    for (auto x = 0; x <= w; ++x)
    {
        DrawPoint(vert);
        vert.u = vert.u + us;
        vert.v = vert.v + vs;
        vert.world = vert.world + ws;
        vert.color = vert.color + cs;
        vert.coord.x = vert.coord.x + 1.0f;
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
    auto index = _buffer.ToIndex(vert.coord.x, vert.coord.y, _bufferWH.w);
    assert(index < _bufferWH.Product());
    if (vert.coord.z < _buffer.zorder[index])
    {
        Color color;
        if (kCOLOR & _drawMode)
        {
            color = vert.color;
        }
        else
        {
            FragmentShader(vert, &color);
        }
        _buffer.frame[index] = color.ToRGBA();
        _buffer.zorder[index] = static_cast<std::uint32_t>(vert.coord.z);
    }
}

void Renderer::VertexShader(const Vertex & v, Vec4 * outv)
{
    _render.param.v = v.coord;
    _render.param.c = v.color;
    _render.param.n = v.normal;
    _render.param.uv.u = v.u;
    _render.param.uv.v = v.v;
    _render.param.caemraPos = _camera.eye;
    _render.material->GetShader().VertexFunc(_render.param);
    *outv = _render.param.outv;
}

void Renderer::FragmentShader(const Vertex & v, Color * outc)
{
    _render.param.v = v.coord;
    _render.param.c = v.color;
    _render.param.n = v.normal;
    _render.param.uv.u = v.u;
    _render.param.uv.v = v.v;
    _render.param.outv = v.world;
    _render.param.caemraPos = _camera.eye;
    _render.material->GetShader().FragmentFunc(_render.param);
    *outc = _render.param.outc;
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

inline bool Renderer::CheckBackCut(const Vec4 & p1, const Vec4 & p2, const Vec4 & p3, Vec4 * outNormal)
{
    auto normal = (p2 - p1).Cross(p3 - p2);
    auto direct = (p1 - _camera.eye);
    if (normal.Dot(direct) >= 0)
    {
        return false;
    }
    *outNormal = normal;
    outNormal->Normal();
    return true;
}
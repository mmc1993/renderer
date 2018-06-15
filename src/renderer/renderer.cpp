#include "renderer.h"

#include "../window/window.h"

class Vec4;
namespace std {
    template <>
    inline std::string to_string(const Vec4 & v4)
    {
        return SFormat("vec4: {0}\n x: {1}\n y: {2}\n z: {3}\n w: {4}", (size_t)&v4, v4.x, v4.y, v4.z, v4.w);
    }
}

void Renderer::SetBufferSize(std::uint32_t w, std::uint32_t h)
{
	_bufferW = w; 
	_bufferH = h;
    _screen.Identity();
    _screen.m[0][0] = w * 0.5f;
    _screen.m[1][1] = h * 0.5f;
    _screen.m[3][0] = w * 0.5f;
    _screen.m[3][1] = h * 0.5f;
    _colorBuffer.reset(new std::uint32_t[w * h]);
    _zorderBuffer.reset(new std::uint32_t[w * h]);
}

void Renderer::SetDrawMode(std::uint8_t mode)
{
    _drawMode = mode;
}

void Renderer::LookAt(const Vec4 & eye, const Vec4 & up, const Vec4 & at)
{
	auto right = up.Cross(at);
	_view.m[0][0] = right.x;
	_view.m[1][0] = right.y;
	_view.m[2][0] = right.z;
	_view.m[3][0] = eye.x;

	_view.m[0][1] = up.x;
	_view.m[1][1] = up.y;
	_view.m[2][1] = up.z;
	_view.m[3][1] = eye.y;

	_view.m[0][2] = at.x;
	_view.m[1][2] = at.y;
	_view.m[2][2] = at.z;
	_view.m[3][2] = eye.z;

	_view.m[0][3] = 0;
	_view.m[1][3] = 0;
	_view.m[2][3] = 0;
	_view.m[3][3] = 1;
    _mVP = _view * _project;

    //  保存eye, 用于背面剔除
    _cameraEye = eye;
}

void Renderer::SetViewPort(std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2)
{
	_viewportX = x1; 
    _viewportY = y1;
	_viewportW = x2 - x1;
    _viewportH = y2 - y1;
}

void Renderer::Primitive(size_t count, Vertex * vertexs, Shader * shader)
{
#ifdef RENDERER_DEBUG
    _debug.vertexCount = count;
    _debug.realVertexCount = 0;
    _debug.triangleCount = count / 3;
    _debug.realTriangleCount = 0;
#endif

    //  初始化着色参数
    _shaderParam.ambientLight.x = 0.2f;
    _shaderParam.ambientLight.y = 0.2f;
    _shaderParam.ambientLight.z = 0.2f;
    _shaderParam.directLight.x = 1.0f;
    _shaderParam.directLight.y = 1.0f;
    _shaderParam.directLight.z = 1.0f;
    _shaderParam.lightPoint.x = 300;
    _shaderParam.lightPoint.y = 300;
    _shaderParam.lightPoint.z = -200;

    _shaderParam.cameraPoint = _cameraEye;
    _shaderParam.lightPoint = _cameraEye;
    _shaderParam.vpMat = _mVP;
    _pRefShader = shader;
	for (auto i = 0; i != count; i += 3)
	{
		Primitive(  vertexs[i	],
			        vertexs[i + 1],
			        vertexs[i + 2]);
	}
    _pRefShader = nullptr;
#ifdef RENDERER_DEBUG
    std::cout 
        << SFormat("顶点总数: {0}, 渲染顶点数: {1}, 三角形数量: {2}, 渲染三角形数量: {3}", 
                   _debug.vertexCount, 
                   _debug.realVertexCount,
                   _debug.triangleCount,
                   _debug.realTriangleCount)
        << std::endl;
#endif
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
    auto diffx = x2 - x1, diffy = y2 - y1;
    auto count = size_t(std::max(std::abs(diffx), 
                                 std::abs(diffy)));
    auto stepx = diffx / count;
    auto stepy = diffy / count;
    for (auto i = 0; i != count; ++i)
    {
        auto idx = (size_t)(y1) * _bufferW + (size_t)(x1);
        _colorBuffer[idx] = _lineRGB;
        x1 += stepx; 
        y1 += stepy;
    }
}

void Renderer::SetLineRGB(const std::uint32_t rgb)
{ 
    _lineRGB = rgb;
}

void Renderer::Primitive(Vertex vert1, Vertex vert2, Vertex vert3)
{
    /*
        问题1, 因为顶点的模型矩阵变换实在传入之前进行的, 因此此时的顶点坐标是世界坐标.
        问题2, 接下来要执行顶点着色器, 正常来说, 应该由顶点着色器传出的坐标进行背面剔除.
    */
    //	背面剔除
    auto normal = (vert2.pt - vert1.pt).Cross(vert3.pt - vert2.pt);
    auto direct = (vert1.pt - _cameraEye);
    if (normal.Dot(direct) > 0) { return ; }
    normal.Normal();

    //  顶点着色器/计算法线/裁剪
    vert1.normal = normal;
    _shaderParam.point = vert1.pt;
    _shaderParam.normal = vert1.normal;
    vert1.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckCut(vert1.pt) != 0) { return ; }

    vert2.normal = normal;
    _shaderParam.point = vert2.pt;
    _shaderParam.normal = vert2.normal;
    vert2.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckCut(vert2.pt) != 0) { return ; }

    vert3.normal = normal;
    _shaderParam.point = vert3.pt;
    _shaderParam.normal = vert3.normal;
    vert3.pt = _pRefShader->VertexFunc(_shaderParam);
    if (CheckCut(vert3.pt) != 0) { return ; }

    //  透视除法
    vert1.pt.x /= vert1.pt.w; vert1.pt.y /= vert1.pt.w; vert1.pt.w /= vert1.pt.w;
    vert2.pt.x /= vert2.pt.w; vert2.pt.y /= vert2.pt.w; vert2.pt.w /= vert2.pt.w;
    vert3.pt.x /= vert3.pt.w; vert3.pt.y /= vert3.pt.w; vert3.pt.w /= vert3.pt.w;

    //  变换到平面坐标
    vert1.pt *= _screen;
    vert2.pt *= _screen;
    vert3.pt *= _screen;

    if (_drawMode & DrawMode::kTEX)
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
    //  y 排列顺序: 0 > 1 > 2
    const Vertex * pVertex[3] = { nullptr };
    //  平顶
    if (vert1.pt.y == vert2.pt.y)
    {
        pVertex[0] = &vert3;
        pVertex[1] = &vert1;
        pVertex[2] = &vert2;
    }
    else if (vert1.pt.y == vert3.pt.y)
    {
        pVertex[0] = &vert2;
        pVertex[1] = &vert1;
        pVertex[2] = &vert3;
    }
    else if (vert2.pt.y == vert3.pt.y)
    {
        pVertex[0] = &vert1;
        pVertex[1] = &vert2;
        pVertex[2] = &vert3;
    }
    else
    {
        Vertex vertexs[3];
        if (vert1.pt.y > vert2.pt.y && vert1.pt.y > vert3.pt.y)
        {
            vertexs[0] = vert1;
            vertexs[1] = vert2.pt.y > vert3.pt.y? vert2: vert3;
            vertexs[2] = vert2.pt.y > vert3.pt.y? vert3: vert2;
        }
        else if (vert2.pt.y > vert1.pt.y && vert2.pt.y > vert3.pt.y)
        {
            vertexs[0] = vert2;
            vertexs[1] = vert1.pt.y > vert3.pt.y? vert1: vert3;
            vertexs[2] = vert1.pt.y > vert3.pt.y? vert3: vert1;
        }
        else if (vert3.pt.y > vert1.pt.y && vert3.pt.y > vert2.pt.y)
        {
            vertexs[0] = vert3;
            vertexs[1] = vert1.pt.y > vert2.pt.y? vert1: vert2;
            vertexs[2] = vert1.pt.y > vert2.pt.y? vert2: vert1;
        }
        auto vert4 = Vertex::LerpFromY(vertexs[0], 
                                       vertexs[2], 
                                       vertexs[1].pt.y - vertexs[0].pt.y);
        vert4.pt.y = vertexs[1].pt.y;
        DrawTriangle(vertexs[0], vert4, vertexs[1]);
        DrawTriangle(vertexs[1], vert4, vertexs[2]);
    }

    if (pVertex[0] != nullptr && pVertex[1] != nullptr && pVertex[2] != nullptr)
    {
        DrawTriangle(pVertex);
    }
}

void Renderer::DrawTriangle(const Vertex ** pVert)
{
    auto y = pVert[0]->pt.y - pVert[1]->pt.y;
    auto l = std::abs(y);
    auto s = y / l;
    for (auto i = 0; i <= l; ++i, y -= s)
    {
        auto start = Vertex::LerpFromY(*pVert[0], *pVert[1], y);
        auto end = Vertex::LerpFromY(*pVert[0], *pVert[2], y);
        DrawScanLine(start, end);
    }
#ifdef RENDERER_DEBUG
    _debug.realTriangleCount += 1;
    _debug.realVertexCount += 3;
#endif
}

void Renderer::DrawScanLine(const Vertex & start, const Vertex & end)
{
    auto w = std::floor(end.pt.x - start.pt.x + 0.5f);
    auto l = std::abs(w);
    auto cs = (end.color - start.color) / l;
    auto us = (end.uv.u - start.uv.u) / l;
    auto vs = (end.uv.v - start.uv.v) / l;
    auto xs = w / l;
    Vertex vert = start;
    for (auto i = 0; i <= l; ++i)
    {
        DrawPoint(vert);
        vert.pt.x = vert.pt.x + xs;
        vert.uv.u = vert.uv.u + us;
        vert.uv.v = vert.uv.v + vs;
        vert.color = vert.color + cs;
    }
}

void Renderer::DrawPoint(const Vertex & vert)
{
    auto idx = (std::uint32_t)vert.pt.y * _bufferW + (std::uint32_t)vert.pt.x;
    assert(idx < _bufferW * _bufferH);
    if (vert.pt.z < _zorderBuffer[idx])
    {
        _shaderParam.u = vert.uv.u;
        _shaderParam.v = vert.uv.v;
        _shaderParam.point = vert.pt;
        _shaderParam.color = vert.color;
        _colorBuffer[idx] = _pRefShader->FragmentFunc(_shaderParam);
        _zorderBuffer[idx] = (std::uint32_t)vert.pt.z;
    }
}

std::uint8_t Renderer::CheckCut(const Vec4 & vec)
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

Renderer::Renderer()
    : _drawMode(DrawMode::kLINE)
{
}

void Renderer::SetFar(float vfar)
{ 
    _far = vfar;
    _project.Identity();
    _project.m[2][3] = 1;
    _project.m[3][0] 
        = _project.m[3][1] 
        = _project.m[3][2] 
        = _project.m[3][3] = 0;
    _mVP = _view * _project;
}

void Renderer::Clear(float r, float g, float b)
{
	std::fill(
		_colorBuffer.get(),
		_colorBuffer.get() + _bufferW * _bufferH, 
		RGB(int(r * 255), int(g * 255), int(b * 255)));

    std::fill(
        _zorderBuffer.get(),
        _zorderBuffer.get() + _bufferW * _bufferH, UINT32_MAX);
}


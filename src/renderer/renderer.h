#pragma once

#include "../base.h"
#include "matrix.h"
#include "shader.h"
#include "color.h"
#include "vec4.h"
#include "math.h"

class Window;

struct Vertex {
	Vec4 pt;
	Vec4 color;
	Vec4 normal;
    struct { float u; float v; } uv;

    static Vertex Lerp(const Vertex & v1, const Vertex & v2, float t)
    {
        Vertex vert;
        vert.pt = Math::Lerp(v1.pt, v2.pt, t);
        vert.uv.u = Math::Lerp(v1.uv.u, v2.uv.u, t);
        vert.uv.v = Math::Lerp(v1.uv.v, v2.uv.v, t);
        vert.color = Math::Lerp(v1.color, v2.color, t);
        return vert;
    }

    static Vertex LerpFromY(const Vertex & v1, const Vertex & v2, float y)
    {
        return Vertex::Lerp(v1, v2, std::abs(1.0f / (v2.pt.y - v1.pt.y) * y));
    }
};

class Renderer {
public:
    enum DrawMode {
        kLINE = 0x1,
        kFILL = 0x2,
    };

    struct Viewport {
        std::uint32_t x;
        std::uint32_t y;
        std::uint32_t w;
        std::uint32_t h;

        Viewport(std::uint32_t _x = 0, std::uint32_t _y = 0, 
                 std::uint32_t _w = 0, std::uint32_t _h = 0)
        {
            x = _x; y = _y;
            w = _w; h = _h;
        }
    };

    struct Size {
        std::uint32_t w;
        std::uint32_t h;

        std::uint32_t Product()
        {
            return w * h;
        }

        Size(std::uint32_t _w = 0, std::uint32_t _h = 0)
        {
            w = _w; h = _h;
        }
    };

    struct Buffer {
    public:
        std::unique_ptr<std::uint32_t[]> frame;
        std::unique_ptr<std::uint32_t[]> zorder;

        void Init(size_t count)
        {
             frame.reset(new std::uint32_t[count]);
             zorder.reset(new std::uint32_t[count]);
            _count = count;
        }

        size_t GetCount()
        {
            return _count;
        }

        size_t ToIndex(float x, float y, std::uint32_t pitch)
        {
            return static_cast<size_t>(y + 0.5f) * pitch + static_cast<size_t>(x + 0.5f);
        }
    private:
        size_t _count;
    };

    struct Transform {
        Matrix4x4 mvp;
        Matrix4x4 view;
        Matrix4x4 model;
        Matrix4x4 project;
        Matrix4x4 screen;
    };

    struct Camera {
        float vfar;
        Vec4 eye;
        Vec4 up;
        Vec4 at;
    };

public:
    Renderer();

    void SetFar(float vfar);

	void Clear(float r, float g, float b);

    void SetDrawMode(std::uint8_t mode);

    void SetLineRGB(const std::uint32_t rgb);

	void SetViewPort(std::uint32_t x1, std::uint32_t y1,
		             std::uint32_t x2, std::uint32_t y2);

	void SetBufferSize(std::uint32_t w, std::uint32_t h);

	void LookAt(const Vec4 & eye, const Vec4 & up, const Vec4 & at);

    void Primitive(size_t count, Vertex * vertexs, Shader * shader);

	std::uint32_t GetBufferW() { return _bufferWH.w; }
	std::uint32_t GetBufferH() { return _bufferWH.h; }
	const std::unique_ptr<std::uint32_t[]> & GetFBufferPtr() { return _buffer.frame; }
    const std::unique_ptr<std::uint32_t[]> & GetZBufferPtr() { return _buffer.zorder; }

private:
	void Primitive(Vertex vert1, Vertex vert2, Vertex vert3);
    void DrawTriangle(const Vertex & vert1, const Vertex & vert2, const Vertex & vert3);
    void DrawTriangle(const Vertex ** pVert);
    void DrawScanLine(const Vertex & start, const Vertex & end);
    void DrawLine(float x1, float y1, float x2, float y2);
    void DrawPoint(const Vertex & vert);

    //   ”Õº≤√ºÙ
    std::uint8_t CheckViewCut(const Vec4 & vec);
    //  ±≥√Ê≤√ºÙ
    bool CheckBackCut(const Vec4 & pt1, 
                      const Vec4 & pt2, 
                      const Vec4 & pt3, Vec4 * outNormal);
private:
    Transform _transform;
    Viewport _viewport;
    Camera _camera;
    Buffer _buffer;
    Size _bufferWH;

    std::uint8_t _drawMode;
    std::uint32_t _lineRGB;

    //  shader
    Shader::Param _shaderParam;
    Shader * _pRefShader;
};
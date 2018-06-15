#pragma once

#include "../base.h"
#include "../math/vec4.h"
#include "../math/matrix.h"
#include "../math/number.h"
#include "shader.h"

class Window;

struct Vertex {
	Vec4 pt;
	Vec4 color;
	Vec4 normal;
    struct { float u; float v; } uv;

    static Vertex Lerp(const Vertex & v1, const Vertex & v2, float t)
    {
        Vertex vert;
        vert.pt = Number::Lerp(v1.pt, v2.pt, t);
        vert.uv.u = Number::Lerp(v1.uv.u, v2.uv.u, t);
        vert.uv.v = Number::Lerp(v1.uv.v, v2.uv.v, t);
        vert.color = Number::Lerp(v1.color, v2.color, t);
        return vert;
    }

    static Vertex LerpFromY(const Vertex & v1, const Vertex & v2, float y)
    {
        return Vertex::Lerp(v1, v2, std::abs(1.0f / (v2.pt.y - v1.pt.y) * y));
    }
};

class Renderer {
#ifdef RENDERER_DEBUG
public:
    struct Debug {
        std::uint32_t vertexCount;
        std::uint32_t realVertexCount;
        std::uint32_t triangleCount;
        std::uint32_t realTriangleCount;
        Debug()
            : vertexCount(0)
            , realVertexCount(0)
            , triangleCount(0)
            , realTriangleCount(0) 
        { }
    };
#endif

public:
    enum DrawMode {
        kLINE = 0x1,
        kTEX = 0x2,
    };

public:
    Renderer();

    void SetFar(float vfar);

	void Clear(float r, float g, float b);

	void SetViewPort(
		std::uint32_t x1, std::uint32_t y1,
		std::uint32_t x2, std::uint32_t y2);

	void SetBufferSize(
		std::uint32_t w, std::uint32_t h);

    void SetDrawMode(std::uint8_t mode);

	void LookAt(
		const Vec4 & eye,
		const Vec4 & up,
		const Vec4 & at);

	void Primitive(size_t count, Vertex * vertexs, Shader * shader);

    void DrawLine(float x1, float y1, float x2, float y2);

    void SetLineRGB(const std::uint32_t rgb);

	std::uint32_t GetBufferW() { return _bufferW; }
	std::uint32_t GetBufferH() { return _bufferH; }
	const std::unique_ptr<std::uint32_t[]> & GetBufferPtr() { return _colorBuffer; }

private:
	void Primitive(Vertex vert1, Vertex vert2, Vertex vert3);

	//	²ð·ÖÈý½ÇÐÎ
    void DrawTriangle(const Vertex & vert1, const Vertex & vert2, const Vertex & vert3);
    void DrawTriangle(const Vertex ** pVert);
    void DrawScanLine(const Vertex & start, const Vertex & end);
    void DrawPoint(const Vertex & vert);

    //  CVV²Ã¼ô
    std::uint8_t CheckCut(const Vec4 & vec);

private:
	Matrix4x4 _mVP;
	Matrix4x4 _view;
    Matrix4x4 _project;
    Matrix4x4 _screen;
    Vec4 _cameraEye;

    float _far;
    std::uint8_t _drawMode;
    std::uint32_t _lineRGB;
	std::uint32_t _bufferW;
	std::uint32_t _bufferH;
	std::uint32_t _viewportX;
	std::uint32_t _viewportY;
	std::uint32_t _viewportW;
	std::uint32_t _viewportH;
	std::unique_ptr<std::uint32_t[]> _colorBuffer;
    std::unique_ptr<std::uint32_t[]> _zorderBuffer;

    //  shader
    Shader::Param _shaderParam;
    Shader * _pRefShader;


#ifdef RENDERER_DEBUG
    Debug _debug;
#endif
};
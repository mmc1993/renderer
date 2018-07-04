#pragma once

#include "window.h"

#include "../renderer/renderer.h"
#include "../renderer/material.h"
#include "../renderer/texture.h"
#include "../renderer/mesh.h"
#include "../renderer/shader/rgb_shader.h"

class AppWindow: public Window {
public:
	AppWindow();
	~AppWindow();

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) override;

private:
	void OnRender();

private:
	Vec4 _cubePoint;
	float _cubeRotateX;
	float _cubeRotateY;
    
    Mesh _mesh;
    Material _material;
	Renderer _renderer;
    RGBShader _rgbShader;
};
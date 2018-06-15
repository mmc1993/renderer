#pragma once

#include "window.h"

#include "../renderer/texture.h"
#include "../renderer/renderer.h"
#include "../renderer/tex_shader.h"
#include "../renderer/rgb_shader.h"

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
    
    Texture _textures[4];
    TexShader _texShader;
    RGBShader _rgbShader;


	Renderer _renderer;
};
#pragma once

#include "window.h"

#include "../renderer/mesh.h"
#include "../renderer/sprite.h"
#include "../renderer/texture.h"
#include "../renderer/material.h"
#include "../renderer/renderer.h"
#include "../renderer/shader/rgb_shader.h"
#include "../cache/material_cache.h"
#include "../cache/shader_cache.h"
#include "../cache/mesh_cache.h"

class AppWindow: public Window {
public:
	AppWindow();
	~AppWindow();

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) override;

private:
	void OnRender();

private:
    Sprite _panle;
    Sprite _cube;

    Renderer _renderer;
    MeshCache _meshCache;
    ShaderCache _shaderCache;
    MaterialCache _materialCache;
};
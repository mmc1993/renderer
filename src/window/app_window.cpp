#include "app_window.h"
#include "../renderer/io/file/file.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnMessage(UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_CREATE:
		{
			SetTimer(GetHwnd(), 1001, 33, nullptr);
			_cubePoint.x = 0;
			_cubePoint.y = 0;
			_cubePoint.z = 25;

            _cubeRotateX = -0;
            _cubeRotateY = -0;

            //  加载纹理
            Material material;
            material.BindShader(&_rgbShader);

            _materialCache.Bind("res/material1", material);

            //  加载网格
            _meshCache.Load("res/1.wrl");

            //  创建精灵
            _sprite.SetCoord(0, 0, 25);
            _sprite.SetMesh(_meshCache.Get("res/1.wrl"));
            _sprite.SetMaterial(_materialCache.Get("res/material1"));
		}
		break;
	case WM_TIMER:
		{
			if (wparam == 1001) 
			{ 
				OnRender(); 
			}
		}
		break;
	case WM_SIZE:
		{
            _renderer.SetFar(2000);
            _renderer.SetLineRGB(RGB(255, 255, 255));
            _renderer.SetDrawMode(/*Renderer::kLINE | */Renderer::kFILL | Renderer::kCOLOR);
			_renderer.SetBufferSize(GetWidth(), GetHeight());
			_renderer.SetViewPort(0, 0, GetWidth(), GetHeight());
			_renderer.LookAt({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 });
		}
		break;
	}
}

void AppWindow::OnRender()
{
	if (GetAsyncKeyState('W') != 0)
	{
        _sprite.SetRotateX(_sprite.GetTransform().rx + 0.1f);
	}
	if (GetAsyncKeyState('S') != 0)
	{
        _sprite.SetRotateX(_sprite.GetTransform().rx - 0.1f);
	}
	if (GetAsyncKeyState('A') != 0)
	{
        _sprite.SetRotateY(_sprite.GetTransform().ry + 0.1f);
	}
	if (GetAsyncKeyState('D') != 0)
	{
        _sprite.SetRotateY(_sprite.GetTransform().ry - 0.1f);
	}

    _renderer.Clear(0, 0, 0);
    _sprite.OnDraw(&_renderer);
    FromRenderer(_renderer);
}

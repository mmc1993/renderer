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
			_cubePoint.z = 50;

            _cubeRotateX = -0;
            _cubeRotateY = -0;

            _material.BindShader(&_rgbShader);
            File::LoadMesh("res/1.wrl", &_mesh);
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
            _renderer.SetDrawMode(Renderer::kLINE | Renderer::kFILL | Renderer::kCOLOR);
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
		_cubeRotateX += 0.1f;
	}
	if (GetAsyncKeyState('S') != 0)
	{
		_cubeRotateX -= 0.1f;
	}
	if (GetAsyncKeyState('A') != 0)
	{
		_cubeRotateY += 0.1f;
	}
	if (GetAsyncKeyState('D') != 0)
	{
		_cubeRotateY -= 0.1f;
	}

    Matrix4x4 transform;
    transform.Identity();
    transform.Rotate(1, 0, 0, _cubeRotateX);
    transform.Rotate(0, 1, 0, _cubeRotateY);
    transform.Translate(_cubePoint.x, _cubePoint.y, _cubePoint.z);

	_renderer.Clear(0.0f, 0.0f, 0.0f);
    _renderer.SetModelMatrix(transform);
    _renderer.Primitive(&_mesh, &_material);
	FromRenderer(&_renderer);
}

#include "app_window.h"
#include "../renderer/light/ambient_light.h"
#include "../renderer/light/direct_light.h"
#include "../renderer/light/point_light.h"
#include "../renderer/light/spot_light.h"

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

            //  环境光
            auto ambientLight = new AmbientLight();
            ambientLight->mColor = Color(0.3f, 0.3f, 0.3f);
            _renderer.AddLight(ambientLight);

            //  方向光
            auto directLight = new DirectLight();
            directLight->SetCoord(Vec4(0, 0, 0));
            directLight->mColor = Color(1.0f, 1.0f, 1.0f);
            _renderer.AddLight(directLight);

            //  点光源
            auto pointLight = new PointLight();
            pointLight->SetCoord(Vec4(5, 0, 13));
            pointLight->mColor = Color(1.0f, 1.0f, 1.0f);
            pointLight->mPower = 5.0f;
            _renderer.AddLight(pointLight);

            //  聚光灯


            //  加载 shader
            _shaderCache.Load<RGBShader>("rgb_shader");

            //  加载纹理
            Material material;
            material.BindShader(_shaderCache.Get("rgb_shader"));
            _materialCache.Bind("res/material1", material);

            //  加载网格
            _meshCache.Load("res/1.wrl");
            _meshCache.Load("res/2.wrl");
            _meshCache.Load("res/3.wrl");

            _root.SetRotateY(90);
            _root.SetCoord(0, 0, 15);
            _root.SetMesh(_meshCache.Get("res/3.wrl"));
            _root.SetMaterial(_materialCache.Get("res/material1"));

            //  创建精灵
            //for (auto i = 0; i != 1; ++i)
            //{
            //    auto sprite = new Sprite();
            //    sprite->SetCoord((i + 1) * 5.0f, 0, 0);
            //    sprite->SetMesh(_meshCache.Get("res/3.wrl"));
            //    sprite->SetMaterial(_materialCache.Get("res/material1"));
            //    _root.AddChild(sprite);
            //}
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
            _renderer.SetDrawMode(/*Renderer::kLINE | */Renderer::kFILL/* | Renderer::kCOLOR*/);
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
        _root.SetRotateX(_root.GetTransform().rx + 5.0f);
	}
	if (GetAsyncKeyState('S') != 0)
	{
        _root.SetRotateX(_root.GetTransform().rx - 5.0f);
	}
	if (GetAsyncKeyState('A') != 0)
	{
        _root.SetRotateY(_root.GetTransform().ry + 5.0f);
	}
	if (GetAsyncKeyState('D') != 0)
	{
        _root.SetRotateY(_root.GetTransform().ry - 5.0f);
	}

    _renderer.Clear(0, 0, 0);
    _root.OnDraw(&_renderer);
    FromRenderer(_renderer);
}

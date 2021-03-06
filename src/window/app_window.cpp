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
            //{
            //    auto pointLight = new PointLight();
            //    pointLight->SetCoord(Vec4(2, 0, 8));
            //    pointLight->mColor = Color(1.0f, 0.0f, 1.0f);
            //    pointLight->mPower = 10.0f;
            //    _renderer.AddLight(pointLight);
            //}

            //{
            //    auto pointLight = new PointLight();
            //    pointLight->SetCoord(Vec4(-3, 2, 11));
            //    pointLight->mColor = Color(1.0f, 1.0f, 1.0f);
            //    pointLight->mPower = 3.0f;
            //    _renderer.AddLight(pointLight);
            //}

            //  聚光灯
            //auto spotLight = new SpotLight();
            //spotLight->SetCoord(Vec4(3, 3, 8));
            //spotLight->mColor = Color(1, 1, 1);
            //spotLight->mDirect = Vec4(0, -1, 0);
            //spotLight->mInRadius = 0.90f;
            //spotLight->mOutRadius = 0.80f;
            //spotLight->mPower = 9.0f;
            //_renderer.AddLight(spotLight);

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

            _panle.SetRotateX(60);
            _panle.SetCoord(0, 0, 10);
            _panle.SetMesh(_meshCache.Get("res/3.wrl"));
            _panle.SetMaterial(_materialCache.Get("res/material1"));

            //_cube.SetRotateX(60);
            //_cube.SetCoord(0, 3, 15);
            //_cube.SetMesh(_meshCache.Get("res/2.wrl"));
            //_cube.SetMaterial(_materialCache.Get("res/material1"));

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
            _renderer.SetDrawMode(Renderer::kLINE/* | Renderer::kFILL*//* | Renderer::kCOLOR*/);
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
        _panle.SetRotateX(_panle.GetTransform().rx + 5.0f);
	}
	if (GetAsyncKeyState('S') != 0)
	{
        _panle.SetRotateX(_panle.GetTransform().rx - 5.0f);
	}
	if (GetAsyncKeyState('A') != 0)
	{
        _panle.SetRotateY(_panle.GetTransform().ry + 5.0f);
	}
	if (GetAsyncKeyState('D') != 0)
	{
        _panle.SetRotateY(_panle.GetTransform().ry - 5.0f);
	}

    _renderer.Clear(0, 0, 0);
    _panle.OnDraw(&_renderer);
    //_cube.OnDraw(&_renderer);
    FromRenderer(_renderer);
}

#include "app_window.h"

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
			_cubePoint.z = 1000;

            //  贴图
            _textures[0].InitFromFile("res/1.png");
            _textures[1].InitFromFile("res/2.png");
            _textures[2].InitFromFile("res/1.png");
            _textures[3].InitFromFile("res/1.png");

            _texShader.BindTexture(0, &_textures[0]);
            _texShader.BindTexture(1, &_textures[1]);
            _texShader.BindTexture(2, &_textures[2]);
            _texShader.BindTexture(3, &_textures[3]);
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
            _renderer.SetEnabledLine(true);
            _renderer.SetLineRGB(RGB(0, 0, 0));
			_renderer.SetBufferSize(GetWidth(), GetHeight());
			_renderer.SetViewPort(0, 0, GetWidth(), GetHeight());
			_renderer.LookAt({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 });
		}
		break;
	}
}

void AppWindow::OnRender()
{
    static const Vertex s_vertexs[] = {
        //  前   0
        { Vec4(-200, -200, -200), Vec4(1, 0, 0), Vec4(), {0, 1} },
        { Vec4(-200, 200, -200), Vec4(0, 1, 0), Vec4(), {0, 0} },
        { Vec4(200, 200, -200), Vec4(1, 0, 0), Vec4(), {1, 0} },
        { Vec4(200, -200, -200), Vec4(0, 0, 1), Vec4(), {1, 1} },

        //  后   4
        { Vec4(-200, -200, 200), Vec4(0, 0, 1), Vec4(), {0, 1} },
        { Vec4(-200, 200, 200), Vec4(1, 0, 0), Vec4(), {0, 0} },
        { Vec4(200, 200, 200), Vec4(0, 1, 0), Vec4(), {1, 0} },
        { Vec4(200, -200, 200), Vec4(1, 0, 0), Vec4(), {1, 1} },

        //  上   8
        { Vec4(-200, 200, -200), Vec4(0, 0, 1), Vec4(), {0, 1} },
        { Vec4(-200, 200, 200), Vec4(1, 0, 0), Vec4(), {0, 0} },
        { Vec4(200, 200, 200), Vec4(0, 1, 0), Vec4(), {1, 0} },
        { Vec4(200, 200, -200), Vec4(1, 0, 0), Vec4(), {1, 1} },

        //  下   12
        { Vec4(-200, -200, 200), Vec4(0, 0, 1), Vec4(), {0, 1} },
        { Vec4(-200, -200, -200), Vec4(1, 0, 0), Vec4(), {0, 0} },
        { Vec4(200, -200, -200), Vec4(0, 1, 0), Vec4(), {1, 0} },
        { Vec4(200, -200, 200), Vec4(1, 0, 0), Vec4(), {1, 1} },

        //  左   16
        { Vec4(-200, -200, 200), Vec4(0, 0, 1), Vec4(), {0, 1} },
        { Vec4(-200, 200, 200), Vec4(1, 0, 0), Vec4(), {0, 0} },
        { Vec4(-200, 200, -200), Vec4(0, 1, 0), Vec4(), {1, 0} },
        { Vec4(-200, -200, -200), Vec4(1, 0, 0), Vec4(), {1, 1} },

        //  右   20
        { Vec4(200, -200, -200), Vec4(0, 0, 1), Vec4(), {0, 1} },
        { Vec4(200, 200, -200), Vec4(1, 0, 0), Vec4(), {0, 0} },
        { Vec4(200, 200, 200), Vec4(0, 1, 0), Vec4(), {1, 0} },
        { Vec4(200, -200, 200), Vec4(1, 0, 0), Vec4(), {1, 1} },
    };

    Vertex vertexs[] = {
        //  前
        s_vertexs[0], s_vertexs[1], s_vertexs[3],
        s_vertexs[1], s_vertexs[2], s_vertexs[3],
        //  后
        s_vertexs[4], s_vertexs[7], s_vertexs[6],
        s_vertexs[4], s_vertexs[6], s_vertexs[5],
        //  左
        s_vertexs[16], s_vertexs[17], s_vertexs[18],
        s_vertexs[16], s_vertexs[18], s_vertexs[19],
        //  右
        s_vertexs[20], s_vertexs[21], s_vertexs[22],
        s_vertexs[20], s_vertexs[22], s_vertexs[23],
        //  上
        s_vertexs[8], s_vertexs[9], s_vertexs[10],
        s_vertexs[8], s_vertexs[10], s_vertexs[11],
        //  下
        s_vertexs[12], s_vertexs[13], s_vertexs[14],
        s_vertexs[12], s_vertexs[14], s_vertexs[15],
    };

	if (GetAsyncKeyState('W') != 0)
	{
		_cubeRotateX += 2.5f;
	}
	if (GetAsyncKeyState('S') != 0)
	{
		_cubeRotateX -= 2.5f;
	}
	if (GetAsyncKeyState('A') != 0)
	{
		_cubeRotateY += 2.5f;
	}
	if (GetAsyncKeyState('D') != 0)
	{
		_cubeRotateY -= 2.5f;
	}

	auto t = Matrix4x4::NewTranslate(_cubePoint.x, _cubePoint.y, _cubePoint.z);
	auto rx = Matrix4x4::NewRotate(1, 0, 0, D2R(_cubeRotateX));
	auto ry = Matrix4x4::NewRotate(0, 1, 0, D2R(_cubeRotateY));
	auto m = t * rx * ry;

	for (auto & vertex : vertexs)
	{
		vertex.pt = vertex.pt * m;
	}

    auto num = sizeof(vertexs) / sizeof(Vertex);
	_renderer.Clear(0.0f, 0.0f, 0.0f);
	_renderer.Primitive(num , vertexs, &_rgbShader);
	FromRenderer(&_renderer);
}

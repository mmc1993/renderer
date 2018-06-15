#include "window.h"

#include "../renderer/renderer.h"

Window::Window()
	: _bufferDC(nullptr)
    , _bufferBM(nullptr)
	, _hwnd(nullptr)
{ }

Window::~Window()
{ }

void Window::Create(const std::string & title)
{
	auto wc = WNDCLASS{ 0 };
	wc.hbrBackground = GetStockBrush(BLACK_BRUSH);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = Window::OnWndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "mmc";
	RegisterClass(&wc);
	
	CreateWindow("mmc", title.c_str(), 
				WS_CAPTION | WS_VISIBLE | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, 
				CW_USEDEFAULT, CW_USEDEFAULT, 
				nullptr, nullptr, nullptr, this);
}

void Window::Move(u_int x, u_int y)
{
	auto w = GetWidth();
	auto h = GetHeight();
	auto window = RECT{ 0 };
	auto client = RECT{ 0 };
	GetWindowRect(_hwnd, &window);
	GetClientRect(_hwnd, &client);
	auto addW = window.right - window.left - client.right;
	auto addH = window.bottom - window.top - client.bottom;
	MoveWindow(_hwnd, x, y, w + addW, h + addH, TRUE);
}

void Window::Size(u_int w, u_int h)
{
	auto window = RECT{ 0 };
	auto client = RECT{ 0 };
	GetWindowRect(_hwnd, &window);
	GetClientRect(_hwnd, &client);
	auto addW = window.right - window.left - client.right;
	auto addH = window.bottom - window.top - client.bottom;
	MoveWindow(_hwnd, window.left, window.top, w + addW, h + addH, TRUE);
}

void Window::Loop()
{
	auto msg = MSG{ 0 };
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

u_int Window::GetWidth()
{
	auto client = RECT{ 0 };
	GetClientRect(_hwnd, &client);
	return client.right - client.left;
}

u_int Window::GetHeight()
{
	auto client = RECT{ 0 };
	GetClientRect(_hwnd, &client);
	return client.bottom - client.top;
}

void Window::InitBuffer()
{
	if (_bufferDC != nullptr)
	{
		DeleteDC(_bufferDC);
	}

	auto hdc = GetDC(_hwnd);
	_bufferDC = CreateCompatibleDC(hdc);
	ReleaseDC(_hwnd, hdc);

    auto bmi = BITMAPINFO{ 0 };
    auto & bmh = static_cast<BITMAPINFOHEADER &>(bmi.bmiHeader);
    bmh.biSize = sizeof(BITMAPINFOHEADER);
    bmh.biPlanes = 1;
    bmh.biBitCount = 32;
    //bmh.biXPelsPerMeter = 96;
    //bmh.biYPelsPerMeter = 96;
    bmh.biWidth = GetWidth();
    bmh.biHeight = GetHeight();
    bmh.biCompression = BI_RGB;
    bmh.biSizeImage = bmh.biWidth * bmh.biHeight  * 4;
    SelectObject(_bufferDC, CreateDIBSection(_bufferDC, &bmi, DIB_RGB_COLORS, &_bufferBM, 0, 0));
}

void Window::FromRenderer(Renderer * prd)
{
    memcpy(_bufferBM, prd->GetBufferPtr().get(), GetWidth() * GetHeight() * 4);

	auto hdc = GetDC(_hwnd);
	BitBlt(hdc, 0, 0, GetWidth(), GetHeight(), _bufferDC, 0, 0, SRCCOPY);
	ReleaseDC(_hwnd, hdc);
}

LRESULT WINAPI Window::OnWndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_CREATE:
		{
			auto param = reinterpret_cast<CREATESTRUCT *>(lparam);
			auto window = reinterpret_cast<Window *>(param->lpCreateParams);
			SetWindowLong(hwnd, GWL_USERDATA, (LONG)window);
			window->_hwnd = hwnd;
			window->InitBuffer();
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	}

	auto udata = GetWindowLong(hwnd, GWL_USERDATA);
	if (udata != 0)
	{
		auto window = reinterpret_cast<Window *>(udata);
		if (window != nullptr)
		{
            if (uint == WM_SIZE)
            {
                window->InitBuffer();
            }
			window->OnMessage(uint, wparam, lparam);
		}
	}

	return DefWindowProc(hwnd, uint, wparam, lparam);
}

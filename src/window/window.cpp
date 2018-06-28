#include "window.h"

#include "../renderer/renderer.h"

Window::Window()
	: _bufferDC(nullptr)
    , _pBufferBM(nullptr)
	, _hwnd(nullptr)
{ }

Window::~Window()
{
    if (_bufferDC != nullptr)
    {
        DeleteDC(_bufferDC);
    }

    if (_bufferBM != nullptr)
    {
        DeleteObject(_bufferBM);
    }
}

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

void Window::Move(std::uint16_t x, std::uint16_t y)
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

void Window::Size(std::uint16_t w, std::uint16_t h)
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

std::uint16_t Window::GetWidth()
{
	auto client = RECT{ 0 };
	GetClientRect(_hwnd, &client);
	return (std::uint16_t)(client.right - client.left);
}

std::uint16_t Window::GetHeight()
{
	auto client = RECT{ 0 };
	GetClientRect(_hwnd, &client);
	return (std::uint16_t)(client.bottom - client.top);
}

void Window::InitBuffer()
{
	if (_bufferDC != nullptr)
	{
		DeleteDC(_bufferDC);
	}

    if (_bufferBM != nullptr)
    {
        DeleteObject(_bufferBM);
    }
    
    auto bmi = BITMAPINFO{ 0 };
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = GetWidth();
    bmi.bmiHeader.biHeight = GetHeight();
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biSizeImage = bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight  * 4;
    _bufferBM = CreateDIBSection(_bufferDC, &bmi, DIB_RGB_COLORS, &_pBufferBM, 0, 0);

    auto hdc = GetDC(_hwnd);
    _bufferDC = CreateCompatibleDC(hdc);
    ReleaseDC(_hwnd, hdc);

    SelectObject(_bufferDC, _bufferBM);
}

void Window::FromRenderer(Renderer * prd)
{
    memcpy(_pBufferBM, prd->GetFBufferPtr().get(), GetWidth() * GetHeight() * 4);

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

#pragma once

#include "../base.h"
#include <Windows.h>
#include <windowsx.h>

class Renderer;

class Window {
public:
	Window();
	virtual ~Window();

	void Create(const std::string & title);
	void Move(std::uint16_t x, std::uint16_t y);
	void Size(std::uint16_t w, std::uint16_t h);
	void Loop();

	std::uint16_t GetWidth();
	std::uint16_t GetHeight();

	HWND GetHwnd() { return _hwnd; }

	void InitBuffer();
	void FromRenderer(Renderer * prd);

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) = 0;

private:
	HWND _hwnd;

	//	buffer
	HDC _bufferDC;
    void *_pBufferBM;
    HBITMAP _bufferBM;

private:
	static LRESULT WINAPI OnWndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
};
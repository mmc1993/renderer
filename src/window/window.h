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
	void Move(u_int x, u_int y);
	void Size(u_int w, u_int h);
	void Loop();

	u_int GetWidth();
	u_int GetHeight();

	HWND GetHwnd() { return _hwnd; }

	void InitBuffer();
	void FromRenderer(Renderer * prd);

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) = 0;

private:
	HWND _hwnd;

	//	buffer
	HDC _bufferDC;

    void *_bufferBM;

private:
	static LRESULT WINAPI OnWndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
};
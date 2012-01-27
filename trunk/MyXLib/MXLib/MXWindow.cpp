#include "stdafx.h"
#include "MXWindow.h"
#include "MXUtils.h"
#include "MXWorld.h"
#include "Windowsx.h"

using namespace MX;



#define WINDOW_CLASS L"MXClass"

namespace MX
{
	LRESULT CALLBACK MXProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window *w = (Window*)GetWindowLong(hWnd, GWL_USERDATA);
		if (w)
			return w->WndProc(message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
};

Window::Window(const wchar_t *name, int w, int h)
{
	WNDCLASSEX WindowClass;
	WindowClass.cbSize = sizeof(WNDCLASSEX); //size of class
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;   //class style
	WindowClass.lpfnWndProc = MXProc;     //window procedure
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = sizeof(Window*);
	WindowClass.hInstance = GetModuleHandle(NULL); //instance
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = WINDOW_CLASS;       //name of class
	WindowClass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&WindowClass);

	hWnd = CreateWindow(WINDOW_CLASS, name, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, w, h, NULL, NULL, WindowClass.hInstance, NULL);

	if (hWnd)
	{
		//set info for callback
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
	}
	Show();
	Update();
}

HINSTANCE Window::hInstance = NULL;

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::Close()
{

}

void Window::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);

}

void Window::Update()
{
	 UpdateWindow(hWnd);
}


struct _WindowMsg
{
	_WindowMsg() { ZeroMemory( &msg, sizeof( msg ) ); }
	MSG msg;
            
} _WndMsg;

bool Window::Loop()
{
	while (_WndMsg.msg.message != WM_QUIT)
	{
		if( PeekMessage( &_WndMsg.msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &_WndMsg.msg );
			DispatchMessage( &_WndMsg.msg );
		}
		else return true;
	}
	return false;
}

LRESULT Window::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_KEYUP:
		World::KeyUp(wParam);
		break;
	case WM_KEYDOWN:
		World::KeyDown(wParam);
		break;

	case WM_LBUTTONDOWN: 
		World::MouseKeyDown(0);
		break;

	case WM_LBUTTONUP: 
		World::MouseKeyUp(0);
		break;

	case WM_RBUTTONUP: 
		World::MouseKeyUp(1);
		break;

	case WM_RBUTTONDOWN: 
		World::MouseKeyDown(1);
		break;

	case WM_MOUSEMOVE: 
		World::MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

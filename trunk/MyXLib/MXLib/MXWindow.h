#ifndef MXLIB_WINDOW
#define MXLIB_WINDOW

#include <windows.h>

namespace MX
{

	class Window
	{
	public:
		Window(const wchar_t *name, int w, int h);
		~Window();

		void Show();
		void Hide();
		void Update();
		void Close();

		static HINSTANCE hInstance;
		static bool Loop();
		HWND GetHWND() { return hWnd; }
	protected:
		friend LRESULT CALLBACK MXProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
		HWND hWnd;
	};

	LRESULT CALLBACK MXProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
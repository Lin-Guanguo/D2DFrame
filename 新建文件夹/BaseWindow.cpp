#include "pch.h"
#include "BaseWindow.h"


void LGG::BaseWindow::create(
	PCWSTR lpWindowName,
	PCWSTR className,
	DWORD dwExStyle,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu)
{
	assert(mhWnd == NULL);
	mhWnd = CreateWindowEx(
		dwExStyle, className, lpWindowName, dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
	);
}

void LGG::BaseWindow::showWindow(int nCmdShow)
{
	assert(mhWnd != NULL);
	ShowWindow(mhWnd, nCmdShow);
}

LRESULT LGG::BaseWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseWindow* pthis;
	if (uMsg == WM_CREATE) {
		CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		pthis = reinterpret_cast<BaseWindow*>(createStruct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pthis));
		pthis->mhWnd = hwnd;
	}
	else {
		pthis = reinterpret_cast<BaseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pthis == NULL) {
		//在 WM_CREATE 之前的消息会到这
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else {
		return pthis->handleMessage(uMsg, wParam, lParam);
	}
}

ATOM LGG::BaseWindow::Rigister(PCWCHAR className)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = BaseWindow::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = className;


	return RegisterClass(&wc);
}

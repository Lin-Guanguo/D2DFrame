#pragma once

namespace LGG
{

class BaseWindow
{
public:
	void showWindow(int nCmdShow);

	HWND gethWnd() const { return mhWnd; }

protected:
	HWND mhWnd = NULL;

	virtual ~BaseWindow() = 0;

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	static ATOM Rigister(PCWCHAR className);

	void create(
		PCWSTR lpWindowName,
		PCWSTR className,
		DWORD dwExStyle = 0,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL,
		HMENU hMenu = NULL);


private:
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);



};

}
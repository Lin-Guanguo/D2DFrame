#include "pch.h"
#include "FrameWindow.h"

#define ReturnOptionOrBreak(option) { if((option).has_value()) return (option).value(); else break; }

LRESULT LGG::FrameWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE: 
		ReturnOptionOrBreak(onCreate(lParam));
	case WM_DESTROY: 
		ReturnOptionOrBreak(onDestory());
	case WM_PAINT:
		ReturnOptionOrBreak(onPaint());
	case WM_KEYDOWN: 
		ReturnOptionOrBreak(onKeyDown(wParam, lParam));
	case WM_KEYUP: 
		ReturnOptionOrBreak(onKeyUp(wParam, lParam));
	case WM_LBUTTONDOWN: 
		ReturnOptionOrBreak(onLeftButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	case WM_LBUTTONUP: 
		ReturnOptionOrBreak(onLeftButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	case WM_RBUTTONDOWN:
		ReturnOptionOrBreak(onRightButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	case WM_RBUTTONUP:
		ReturnOptionOrBreak(onRightButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	} // switch (uMsg)
	return DefWindowProc(mhWnd, uMsg, wParam, lParam);
}

std::optional<LRESULT> LGG::FrameWindow::onDestory()
{
	PostQuitMessage(0);
	return 0;
}

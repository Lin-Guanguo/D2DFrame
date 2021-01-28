#pragma once
#include "BaseWindow.h"

namespace LGG
{

class FrameWindow : public BaseWindow
{
protected:
	virtual ~FrameWindow() = 0;

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//WM_CREATE
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onCreate(LPARAM lParam) { return std::nullopt; }

	//WM_DESTORY
	//default operation: call PostQuitMessage(0); return 0;
	virtual std::optional<LRESULT> onDestory();

	//WM_PAINT
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onPaint() { return std::nullopt; }

	//WM_KEYDOWN
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onKeyDown(WPARAM virtualKey, LPARAM flag) { return std::nullopt; };

	//WM_KEYUP
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onKeyUp(WPARAM virtualKey, LPARAM flag) { return std::nullopt; };

	//WM_LBUTTONDOWN
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onLeftButtonDown(WPARAM virtualKeyState, int x, int y) { return std::nullopt; };

	//WM_LBUTTONUP
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onLeftButtonUp(WPARAM virtualKeyState, int x, int y) { return std::nullopt; };

	//WM_RBUTTONDOWN
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onRightButtonDown(WPARAM virtualKeyState, int x, int y) { return std::nullopt; };
	
	//WM_RBUTTONUP
	//default operation: call DefWindowProc()
	virtual std::optional<LRESULT> onRightButtonUp(WPARAM virtualKeyState, int x, int y) { return std::nullopt; };

};

}

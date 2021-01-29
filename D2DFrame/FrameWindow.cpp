#include "pch.h"
#include "FrameWindow.h"
#include "IUpdateWithTime.h"
#include "Util.h"

#define ReturnOptionOrBreak(option) { auto&& ReturnOptionOrBreak_o = (option); if(ReturnOptionOrBreak_o.has_value()) return ReturnOptionOrBreak_o.value(); else break; }

void LGG::FrameWindow::run()
{
	mMainTimer.reset();
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			mMainTimer.tick();
			runEveryLoop();
			calculateFrameStats();
			update();
			reRender();
		}
	}
}

void LGG::FrameWindow::update()
{
	mUpdateList.update(mMainTimer);
}

void LGG::FrameWindow::reRender()
{
	InvalidateRect(mhWnd, NULL, FALSE);
}

void LGG::FrameWindow::calculateFrameStats()
{
	mFrameCntForCalculateFrameStats++;
	if ((mMainTimer.totalTime() - mTimeElapsedForCalculateFrameStats) >= 1.0f)
	{
		float fps = (float)mFrameCntForCalculateFrameStats;
		mFrameCntForCalculateFrameStats = 0;
		++mTimeElapsedForCalculateFrameStats;
	}
}

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

std::optional<LRESULT> LGG::FrameWindow::onCreate(LPARAM lParam)
{
	mD2DGraphicsList.initialization();
	return 0;
}

std::optional<LRESULT> LGG::FrameWindow::onDestory()
{
	PostQuitMessage(0);
	return 0;
}

std::optional<LRESULT> LGG::FrameWindow::onPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(mhWnd, &ps);
	{
		mD2DGraphicsList.render(mhWnd);
	}
	EndPaint(mhWnd, &ps);
	return 0;
}

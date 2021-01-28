#include "pch.h"
#include "GameWindow.h"
#include "FlappyBird.h"
#include "constants.h"
#include "IUpdateWithTime.h"
#include "Util.h"

LGG::GameWindow::GameWindow()
	: mGraphFactory(GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y),
	mFlappyBird(std::make_shared<FlappyBird>())
{
	mGraphFactory.addSubGraphOnTop(mFlappyBird);
	mUpdateList.emplace_back(mFlappyBird);

	BaseWindow::Rigister(WINDOW_CLASS_NAME);
	BaseWindow::create(
		WINDOW_NAME,
		WINDOW_CLASS_NAME,
		0,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GAME_SCREEN_SIZE_X * 2,
		GAME_SCREEN_SIZE_Y * 2
		);
}

void LGG::GameWindow::run()
{
	mMainTimer.reset();
	MSG msg = {};
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			mMainTimer.tick();
			CalculateFrameStats();
			update();
			reRender();
		}
	}
}

LRESULT LGG::GameWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_CREATE:
	{
		mGraphFactory.initialization();
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(mhWnd, &ps);
		{
			mGraphFactory.render(mhWnd);
		}
		EndPaint(mhWnd, &ps);
		return 0;
	}
	case WM_KEYDOWN:
	{
		auto r = onKeyDown(wParam, lParam);
		if (r.has_value()) return r.value();
		break;
	}
	case WM_LBUTTONDOWN:
	{
		auto r = onLeftButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (r.has_value()) return r.value();
		break;
	}
	case WM_RBUTTONDOWN:
	{
		auto r = onRightButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (r.has_value()) return r.value();
		break;
	}
	} // switch (uMsg)
	return DefWindowProc(mhWnd, uMsg, wParam, lParam);
}

void LGG::GameWindow::update()
{
	auto& t = this->mMainTimer;
	mUpdateList.forEachAndClean(
		[&t](IUpdateWithTime& o) {
			o.update(t);
		}
	);
}

void LGG::GameWindow::reRender()
{
	InvalidateRect(mhWnd, NULL, FALSE);
}

void LGG::GameWindow::CalculateFrameStats()
{
	static int frameCnt = 0;

	frameCnt++;

	if ((mMainTimer.totalTime() - mTimeElapsedForCalculateFrameStats) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		std::wstring windowText = std::wstring(WINDOW_NAME) +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;

		SetWindowText(mhWnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		++mTimeElapsedForCalculateFrameStats;
	}
}

std::optional<LRESULT> LGG::GameWindow::onKeyDown(WPARAM virtualKey, LPARAM flag)
{
	switch (virtualKey) {
	case VK_UP:
	{
		mFlappyBird->startFly();
		mFlappyBird->fly();
		return 0;
	}
	case 'R':
	{
		mFlappyBird->reset();
		return 0;
	}
	case 'D':
	{
		mFlappyBird->setState(FlappyBird::BirdState::DIE);
		return 0;
	}
	default:
	{
		return std::nullopt;
	}
	}
}

std::optional<LRESULT> LGG::GameWindow::onLeftButtonDown(WPARAM virtualKeyState, int x, int y)
{
	auto rate = mGraphFactory.pointWindowToRenderTargetRate(mhWnd);
	DebugOutPut(L"LeftClick ", x * rate.x, L" ", y * rate.y);
	return std::nullopt;
}

std::optional<LRESULT> LGG::GameWindow::onRightButtonDown(WPARAM virtualKeyState, int x, int y)
{

	return std::nullopt;
}


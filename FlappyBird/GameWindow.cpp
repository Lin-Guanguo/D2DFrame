#include "pch.h"
#include "GameWindow.h"
#include "FlappyBird.h"
#include "constants.h"
#include "IUpdateWithTime.h"
#include "Util.h"

LGG::GameWindow::GameWindow()
	: FrameWindow({ GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y }),
	mFlappyBird(std::make_shared<FlappyBird>())
{
	mD2DGraphicsList.emplace_back(mFlappyBird);
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
	auto rate = mD2DGraphicsList.pointWindowToRenderTargetRate(mhWnd);
	DebugOutPut(L"LeftClick ", x * rate.x, L" ", y * rate.y);
	return std::nullopt;
}

std::optional<LRESULT> LGG::GameWindow::onRightButtonDown(WPARAM virtualKeyState, int x, int y)
{

	return std::nullopt;
}


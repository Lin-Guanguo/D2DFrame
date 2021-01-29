#pragma once


#include "BaseWindow.h"
#include "FrameWindow.h"

namespace LGG 
{

class FlappyBird;

class GameWindow : public FrameWindow
{
private:
	std::shared_ptr<FlappyBird> mFlappyBird;

public:
	static constexpr PCWCHAR WINDOW_CLASS_NAME = L"GameWindow";
	static constexpr PCWCHAR WINDOW_NAME = L"test Winodw";

	GameWindow();

private:
	//处理 键盘 事件
	virtual std::optional<LRESULT> onKeyDown(WPARAM virtualKey, LPARAM flag) override;

	virtual std::optional<LRESULT> onLeftButtonDown(WPARAM virtualKeyState, int x, int y) override;

	virtual std::optional<LRESULT> onRightButtonDown(WPARAM virtualKeyState, int x, int y) override;

};

}
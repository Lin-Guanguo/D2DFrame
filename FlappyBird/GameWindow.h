#pragma once

#include "BaseWindow.h"
#include "GameTimer.h"
#include "D2DFactory.h"
#include "IUpdateWithTime.h"
#include "Util.h"

namespace LGG 
{

class FlappyBird;

class GameWindow : public BaseWindow
{
private:
	GameTimer mMainTimer;
	int mTimeElapsedForCalculateFrameStats = 0;

	D2DFactory mGraphFactory;
	WeakPtrList<IUpdateWithTime> mUpdateList;

	std::shared_ptr<FlappyBird> mFlappyBird;

public:
	static constexpr PCWCHAR WINDOW_CLASS_NAME = L"GameWindow";
	static constexpr PCWCHAR WINDOW_NAME = L"test Winodw";

	GameWindow();

	//�������¼�ѭ��
	void run();

private:
	//win32�¼�����
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��������
	void update();

	//��Ⱦͼ��
	void reRender();

	//����FPS�ȿ����Ϣ
	void CalculateFrameStats();

	//���� ���� �¼�
	std::optional<LRESULT> onKeyDown(WPARAM virtualKey, LPARAM flag);

	std::optional<LRESULT> onLeftButtonDown(WPARAM virtualKeyState, int x, int y);

	std::optional<LRESULT> onRightButtonDown(WPARAM virtualKeyState, int x, int y);

};

}
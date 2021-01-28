#pragma once
#include "BaseWindow.h"
#include "GameTimer.h"
#include "D2DFactory.h"
#include "Util.h"

namespace LGG
{

class IUpdateWithTime;

class FrameWindow : public BaseWindow
{
protected:

	//主计时器
	GameTimer mMainTimer;

	//DirectX2D管理工厂
	//将实现了ID2DGraph接口的组件放入其中，在事件循环中自动绘制。
	D2DFactory mD2DFactory;

	//数据更新管理
	//将实现了IUpdateWithTime接口的组件放入其中，在事件循环中根据定时器更新。
	WeakPtrList<IUpdateWithTime> mUpdateList;

	//帧率统计
	float mFps;

public:
	virtual ~FrameWindow() = 0;

	virtual void run();

private:
	void update();

	void reRender();

	void calculateFrameStats();
	int mTimeElapsedForCalculateFrameStats = 0;
	int mFrameCntForCalculateFrameStats = 0;

protected:
	//run in every loop
	//with update(), reRender() and calculateFrameStats()
	//default operation: do nothing
	virtual void runEveryLoop() { }

	//事件分发函数，调用一系列 on**() 函数
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//WM_CREATE
	//default operation: call mD2DFactory.initialization(); return 0;
	virtual std::optional<LRESULT> onCreate(LPARAM lParam);

	//WM_DESTORY
	//default operation: call PostQuitMessage(0); return 0;
	virtual std::optional<LRESULT> onDestory();

	//WM_PAINT
	//default operation: call mD2DFactory.render(HWND);
	virtual std::optional<LRESULT> onPaint();

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

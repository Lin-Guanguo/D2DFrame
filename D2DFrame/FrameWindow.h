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

	//����ʱ��
	GameTimer mMainTimer;

	//DirectX2D������
	//��ʵ����ID2DGraph�ӿڵ�����������У����¼�ѭ�����Զ����ơ�
	D2DFactory mD2DFactory;

	//���ݸ��¹���
	//��ʵ����IUpdateWithTime�ӿڵ�����������У����¼�ѭ���и��ݶ�ʱ�����¡�
	WeakPtrList<IUpdateWithTime> mUpdateList;

	//֡��ͳ��
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

	//�¼��ַ�����������һϵ�� on**() ����
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

#pragma once
#include "WeakPtrList.h"

namespace LGG
{

class ID2DGraphics;

class D2DGraphicsList : public WeakPtrList<ID2DGraphics>
{
private:

	CComPtr<ID2D1Factory>  mFactory;
    CComPtr<ID2D1HwndRenderTarget> mRenderTarget;
    
    D2D1_SIZE_U mSizeofRenderTarget;
public:
    D2DGraphicsList(D2D1_SIZE_U renderTargetSize = {100u, 100u});

    ~D2DGraphicsList();

    //���� ���� �� �豸�޹���Դ
    void initialization();

    //�ı�renderTarget�Ĵ�С
    //renderTarget���Զ������������������ڣ�
    //����ϣ��������Ч������ WM_SIZE �е��ø÷��� resize
    void resize(int x, int y);

    void render(HWND hwnd);

    //���� DPI �� ���ڴ�С����ȾĿ���С���� ת������
    //ʹ�ó���: ��ȾĿ�� ����� ���� ������
    D2D1_POINT_2F pointWindowToRenderTargetRate(HWND hwnd);
    
    //���� DPI ת������
    //ʹ�ó���: ��ȾĿ���С�ʹ��ڴ�Сһ��, ����ת��ֻ�غ�DPI
    static float dpiWindowToRenderTargetRate(HWND hwnd);

private:
    //���� �豸�޹ص���Դ, ֻ�ᱻinitialization()����һ��
    void createDeviceIndependentResources();

    //�ͷ� �豸�޹ص���Դ, ֻ�ᱻ~D2DGraphicsList()����һ��
    void discardDeviceIndependentResources();

    //���� �豸��������Դ, ���豸�仯ʱ(��ʼ��ʱ)������
    void createDeviceDependentResources(HWND hwnd);

    //�ͷ� �豸��������Դ, ���豸�仯ʱ(����ʱ)������
    void discardDeviceDependentResources();
};

}

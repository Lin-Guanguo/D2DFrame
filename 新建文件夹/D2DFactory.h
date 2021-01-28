#pragma once
#include "ID2DGraph.h"
#include "Util.h"

namespace LGG
{

class D2DFactory
{
private:

	CComPtr<ID2D1Factory>  mFactory;
    CComPtr<ID2D1HwndRenderTarget> mRenderTarget;
    
    D2D1_SIZE_U mSizeofRenderTarget;
    
	WeakPtrList<ID2DGraph> mSubGraph;
public:
    //��Ĭ����Ⱦ��С����
	D2DFactory() : D2DFactory(100, 100) { };

	D2DFactory(int x, int y);

    ~D2DFactory();

    //���� ���� �� �豸�޹���Դ
    void initialization();

    //�ı�renderTarget�Ĵ�С
    //renderTarget���Զ������������������ڣ�
    //����ϣ��������Ч������ WM_SIZE �е��ø÷��� resize
    void resize(int x, int y);

    void render(HWND hwnd);

    //�ڵײ������ͼ��
    void addSubGraphOnTop(std::weak_ptr<ID2DGraph> subGraph);

    //�ڶ��������ͼ��
    void addSubGraphOnButtom(std::weak_ptr<ID2DGraph> subGraph);

    //���ͼ��
    void clearSubGraph();

    //���� DPI �� ���ڴ�С����ȾĿ���С���� ת������
    //ʹ�ó���: ��ȾĿ�� ����� ���� ������
    D2D1_POINT_2F pointWindowToRenderTargetRate(HWND hwnd);
    
    //���� DPI ת������
    //ʹ�ó���: ��ȾĿ���С�ʹ��ڴ�Сһ��, ����ת��ֻ�غ�DPI
    static float dpiWindowToRenderTargetRate(HWND hwnd);

private:
    //���� �豸�޹ص���Դ, ֻ�ᱻinitialization()����һ��
    void createDeviceIndependentResources();

    //�ͷ� �豸�޹ص���Դ, ֻ�ᱻ~D2DFactory()����һ��
    void discardDeviceIndependentResources();

    //���� �豸��������Դ, ���豸�仯ʱ(��ʼ��ʱ)������
    void createDeviceDependentResources(HWND hwnd);

    //�ͷ� �豸��������Դ, ���豸�仯ʱ(����ʱ)������
    void discardDeviceDependentResources();
};

}

#pragma once

namespace LGG
{

    class ID2DGraphics
    {
    public:
        virtual void render(CComPtr<ID2D1HwndRenderTarget> renderTarget) = 0;

        //���� �豸�޹ص���Դ
        virtual void createDeviceIndependentResources(CComPtr<ID2D1Factory> factory) = 0;

        //�ͷ� �豸�޹ص���Դ
        virtual void discardDeviceIndependentResources() = 0;

        //���� �豸��������Դ
        virtual void createDeviceDependentResources(CComPtr<ID2D1HwndRenderTarget> renderTarget) = 0;

        //�ͷ� �豸��������Դ
        virtual void discardDeviceDependentResources() = 0;

    };

}

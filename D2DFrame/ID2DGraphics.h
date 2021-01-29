#pragma once

namespace LGG
{

    class ID2DGraphics
    {
    public:
        virtual void render(CComPtr<ID2D1HwndRenderTarget> renderTarget) = 0;

        //建立 设备无关的资源
        virtual void createDeviceIndependentResources(CComPtr<ID2D1Factory> factory) = 0;

        //释放 设备无关的资源
        virtual void discardDeviceIndependentResources() = 0;

        //建立 设备依赖的资源
        virtual void createDeviceDependentResources(CComPtr<ID2D1HwndRenderTarget> renderTarget) = 0;

        //释放 设备依赖的资源
        virtual void discardDeviceDependentResources() = 0;

    };

}

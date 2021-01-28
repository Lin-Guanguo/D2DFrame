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
    //以默认渲染大小创建
	D2DFactory() : D2DFactory(100, 100) { };

	D2DFactory(int x, int y);

    ~D2DFactory();

    //建立 工厂 和 设备无关资源
    void initialization();

    //改变renderTarget的大小
    //renderTarget会自动拉伸至覆盖整个窗口，
    //若不希望有拉伸效果可在 WM_SIZE 中调用该方法 resize
    void resize(int x, int y);

    void render(HWND hwnd);

    //在底部添加新图像
    void addSubGraphOnTop(std::weak_ptr<ID2DGraph> subGraph);

    //在顶层添加新图像
    void addSubGraphOnButtom(std::weak_ptr<ID2DGraph> subGraph);

    //清空图像
    void clearSubGraph();

    //根据 DPI 和 窗口大小与渲染目标大小比例 转换坐标
    //使用场景: 渲染目标 相比于 窗口 有拉伸
    D2D1_POINT_2F pointWindowToRenderTargetRate(HWND hwnd);
    
    //根据 DPI 转换坐标
    //使用场景: 渲染目标大小和窗口大小一致, 坐标转换只关乎DPI
    static float dpiWindowToRenderTargetRate(HWND hwnd);

private:
    //建立 设备无关的资源, 只会被initialization()调用一次
    void createDeviceIndependentResources();

    //释放 设备无关的资源, 只会被~D2DFactory()调用一次
    void discardDeviceIndependentResources();

    //建立 设备依赖的资源, 当设备变化时(初始化时)被调用
    void createDeviceDependentResources(HWND hwnd);

    //释放 设备依赖的资源, 当设备变化时(析构时)被调用
    void discardDeviceDependentResources();
};

}

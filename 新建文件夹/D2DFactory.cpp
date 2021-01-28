#include "pch.h"
#include "D2DFactory.h"
#include "Util.h"

LGG::D2DFactory::D2DFactory(int x, int y)
    :mSizeofRenderTarget(D2D1::SizeU(x,y))
{

}

LGG::D2DFactory::~D2DFactory()
{
    discardDeviceDependentResources();
    discardDeviceIndependentResources();
}

void LGG::D2DFactory::initialization()
{
    createDeviceIndependentResources();
}

void LGG::D2DFactory::resize(int x, int y)
{
    mSizeofRenderTarget = D2D1::SizeU(x, y);
    if (mRenderTarget) {
        THROW_ON_FAILED(
            mRenderTarget->Resize(mSizeofRenderTarget)
        );
    }
}

void LGG::D2DFactory::render(HWND hwnd)
{
    createDeviceDependentResources(hwnd);

    assert(mRenderTarget != NULL);

    mRenderTarget->BeginDraw();
    {
        mSubGraph.forEachAndClean(
            [renderTarget = mRenderTarget](ID2DGraph& g) {
                g.render(renderTarget);
            }
        );
    }
    auto hr = mRenderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET)
    {
        discardDeviceDependentResources();
    }
    else {
        THROW_ON_FAILED(hr);
    }
}

void LGG::D2DFactory::addSubGraphOnTop(std::weak_ptr<ID2DGraph> subGraph)
{
    mSubGraph.emplace_back(subGraph);
}

void LGG::D2DFactory::addSubGraphOnButtom(std::weak_ptr<ID2DGraph> subGraph)
{
    mSubGraph.emplace_front(subGraph);
}

void LGG::D2DFactory::clearSubGraph()
{
    mSubGraph.clear();
}

D2D1_POINT_2F LGG::D2DFactory::pointWindowToRenderTargetRate(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    auto dpi = GetDpiForWindow(hwnd);
    float ratex = 96.0f / dpi * (static_cast<float>(mSizeofRenderTarget.width) / (rect.right - rect.left));
    float ratey = 96.0f / dpi * (static_cast<float>(mSizeofRenderTarget.height) / (rect.bottom - rect.top));
    return D2D1::Point2F(ratex, ratey);
}

float LGG::D2DFactory::dpiWindowToRenderTargetRate(HWND hwnd)
{
    auto dpi = GetDpiForWindow(hwnd);
    return 96.0f / dpi;
}

void LGG::D2DFactory::createDeviceIndependentResources()
{
    THROW_ON_FAILED(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &mFactory)
    );

    mSubGraph.forEachAndClean(
        [factory = mFactory](ID2DGraph& g) {
            g.createDeviceIndependentResources(factory);
        }
    );
}

void LGG::D2DFactory::discardDeviceIndependentResources()
{
    mSubGraph.forEachAndClean(
        [](ID2DGraph& g) {
            g.discardDeviceIndependentResources();
        }
    );
}

void LGG::D2DFactory::createDeviceDependentResources(HWND hwnd)
{
    if (mRenderTarget == NULL)
    {
        THROW_ON_FAILED(
            mFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hwnd, mSizeofRenderTarget),
                &mRenderTarget)
        );


        mSubGraph.forEachAndClean(
            [renderTarget = mRenderTarget](ID2DGraph& g) {
                g.createDeviceDependentResources(renderTarget);
            }
        );
    }
}

void LGG::D2DFactory::discardDeviceDependentResources()
{
    mSubGraph.forEachAndClean(
        [](ID2DGraph& g) {
            g.discardDeviceDependentResources();
        }
    );
    mRenderTarget.Release();
}

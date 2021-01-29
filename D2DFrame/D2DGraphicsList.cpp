#include "pch.h"
#include "D2DGraphicsList.h"
#include "ID2DGraphics.h"
#include "Util.h"

LGG::D2DGraphicsList::D2DGraphicsList(D2D1_SIZE_U renderTargetSize)
    :mSizeofRenderTarget(renderTargetSize)
{
}

LGG::D2DGraphicsList::~D2DGraphicsList()
{
    discardDeviceDependentResources();
    discardDeviceIndependentResources();
}

void LGG::D2DGraphicsList::initialization()
{
    createDeviceIndependentResources();
}

void LGG::D2DGraphicsList::resize(int x, int y)
{
    mSizeofRenderTarget = D2D1::SizeU(x, y);
    if (mRenderTarget) {
        THROW_ON_FAILED(
            mRenderTarget->Resize(mSizeofRenderTarget)
        );
    }
}

void LGG::D2DGraphicsList::render(HWND hwnd)
{
    createDeviceDependentResources(hwnd);

    assert(mRenderTarget != NULL);

    mRenderTarget->BeginDraw();
    {
        this->forEachAndClean(
            [renderTarget = mRenderTarget](ID2DGraphics& g) {
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

D2D1_POINT_2F LGG::D2DGraphicsList::pointWindowToRenderTargetRate(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    auto dpi = GetDpiForWindow(hwnd);
    float ratex = 96.0f / dpi * (static_cast<float>(mSizeofRenderTarget.width) / (rect.right - rect.left));
    float ratey = 96.0f / dpi * (static_cast<float>(mSizeofRenderTarget.height) / (rect.bottom - rect.top));
    return D2D1::Point2F(ratex, ratey);
}

float LGG::D2DGraphicsList::dpiWindowToRenderTargetRate(HWND hwnd)
{
    auto dpi = GetDpiForWindow(hwnd);
    return 96.0f / dpi;
}

void LGG::D2DGraphicsList::createDeviceIndependentResources()
{
    THROW_ON_FAILED(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &mFactory)
    );

    this->forEachAndClean(
        [factory = mFactory](ID2DGraphics& g) {
            g.createDeviceIndependentResources(factory);
        }
    );
}

void LGG::D2DGraphicsList::discardDeviceIndependentResources()
{
    this->forEachAndClean(
        [](ID2DGraphics& g) {
            g.discardDeviceIndependentResources();
        }
    );
}

void LGG::D2DGraphicsList::createDeviceDependentResources(HWND hwnd)
{
    if (mRenderTarget == NULL)
    {
        THROW_ON_FAILED(
            mFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hwnd, mSizeofRenderTarget),
                &mRenderTarget)
        );


        this->forEachAndClean(
            [renderTarget = mRenderTarget](ID2DGraphics& g) {
                g.createDeviceDependentResources(renderTarget);
            }
        );
    }
}

void LGG::D2DGraphicsList::discardDeviceDependentResources()
{
    this->forEachAndClean(
        [](ID2DGraphics& g) {
            g.discardDeviceDependentResources();
        }
    );
    mRenderTarget.Release();
}

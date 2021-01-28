#pragma once
#include "Util.h"

namespace LGG
{

namespace BitmapLoader
{
    void LoadBitmapFromFile(
        ID2D1RenderTarget* pRenderTarget,
        PCWSTR uri,
        ID2D1Bitmap** ppBitmap
    );

    void LoadBitmapFromResource(
        ID2D1RenderTarget* pRenderTarget,
        int resourceID,
        PCWSTR resourceType,
        ID2D1Bitmap** ppBitmap
    );
}

}

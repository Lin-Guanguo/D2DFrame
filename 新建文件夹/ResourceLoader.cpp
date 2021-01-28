#include "pch.h"
#include "ResourceLoader.h"
#include "Util.h"

namespace 
{

CComPtr<IWICImagingFactory> gIWICFactory = NULL;

void initWICFactory() {
    auto hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&gIWICFactory)
    );
    THROW_ON_FAILED(hr);
}

}

void LGG::BitmapLoader::LoadBitmapFromFile(
    ID2D1RenderTarget* pRenderTarget,
    PCWSTR uri,
    ID2D1Bitmap** ppBitmap
)
{
    if (!gIWICFactory) { initWICFactory(); }

    CComPtr<IWICBitmapDecoder> pDecoder;
    HRESULT hr = gIWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    THROW_ON_FAILED(hr);

    // Create the initial frame.
    CComPtr<IWICBitmapFrameDecode> pSource;
    hr = pDecoder->GetFrame(0, &pSource);
    THROW_ON_FAILED(hr);

    // Convert the image format to 32bppPBGRA
    // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
    CComPtr<IWICFormatConverter> pConverter;
    hr = gIWICFactory->CreateFormatConverter(&pConverter);
    THROW_ON_FAILED(hr);

    hr = pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );
    THROW_ON_FAILED(hr);

    // Create a Direct2D bitmap from the WIC bitmap.
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
        pConverter,
        NULL,
        ppBitmap
    );
    THROW_ON_FAILED(hr);
}

void LGG::BitmapLoader::LoadBitmapFromResource(
    ID2D1RenderTarget* pRenderTarget, 
    int resourceID,
    PCWSTR resourceType,
    ID2D1Bitmap** ppBitmap
)
{
    if (!gIWICFactory) { initWICFactory(); }

    HMODULE HINST_THISCOMPONENT = NULL;
    auto resourceName = MAKEINTRESOURCE(resourceID);

    // Locate the resource.
    HRSRC imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
    THROW_ON_FAILED(imageResHandle ? S_OK : E_FAIL);

    // Load the resource.
    HGLOBAL imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);
    THROW_ON_FAILED(imageResDataHandle ? S_OK : E_FAIL);

    // Lock it to get a system memory pointer.
    void* pImageFile = LockResource(imageResDataHandle);
    THROW_ON_FAILED(pImageFile ? S_OK : E_FAIL);

    // Calculate the size.
    DWORD imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);
    THROW_ON_FAILED(imageFileSize ? S_OK : E_FAIL);

    // Create a WIC stream to map onto the memory.
    CComPtr<IWICStream> pStream;
    auto hr = gIWICFactory->CreateStream(&pStream);
    THROW_ON_FAILED(hr);

    // Initialize the stream with the memory pointer and size.
    hr = pStream->InitializeFromMemory(
        reinterpret_cast<BYTE*>(pImageFile),
        imageFileSize
    );
    THROW_ON_FAILED(hr);

    // Create a decoder for the stream.
    CComPtr<IWICBitmapDecoder> pDecoder;
    hr = gIWICFactory->CreateDecoderFromStream(
        pStream,
        NULL,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    THROW_ON_FAILED(hr);

    // Create the initial frame.
    CComPtr<IWICBitmapFrameDecode> pSource;
    hr = pDecoder->GetFrame(0, &pSource);
    THROW_ON_FAILED(hr);

    // Convert the image format to 32bppPBGRA
    // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
    CComPtr<IWICFormatConverter> pConverter;
    hr = gIWICFactory->CreateFormatConverter(&pConverter);
    THROW_ON_FAILED(hr);

    hr = pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );
    THROW_ON_FAILED(hr);

    //create a Direct2D bitmap from the WIC bitmap.
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
        pConverter,
        NULL,
        ppBitmap
    );
    THROW_ON_FAILED(hr);
}

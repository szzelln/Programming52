// D2DRenderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
// D2DRenderer : D2D관련 인터페이스

#include "pch.h"
#include "framework.h"
#include "D2DRenderer.h"


// TODO: 라이브러리 함수의 예제입니다.
void fnD2DRenderer()
{
}

D2DRenderer::D2DRenderer(HWND hWnd)
{
   g_hWnd = hWnd;
}

D2DRenderer::~D2DRenderer()
{
}



bool D2DRenderer::Initialize()
{
    HRESULT hr;

    // COM사용
    hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        // 렌더타겟을 만들수있는팩토리객체를 생성하고 인터페이스 포인터를얻어온다.
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    }

    if (SUCCEEDED(hr))
    {
        // 이미 윈도우가만들어진 상태에서 윈도우 사이즈를 구한다.
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            static_cast<UINT>(rc.right - rc.left),
            static_cast<UINT>(rc.bottom - rc.top)
        );

        // 팩토리로 윈도우핸들,사이즈를 넘겨 렌더카겟을 만든다.
        hr = g_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(g_hWnd, size),
            &g_pD2DRenderTarget
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = g_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &g_pGrayBrush);
    }

    if (SUCCEEDED(hr))
    {
        hr = g_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &g_pBlackBrush);
    }

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(g_pDWriteFactory),
            reinterpret_cast<IUnknown**>(&g_pDWriteFactory)
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = g_pDWriteFactory->CreateTextFormat(
            L"Bernard MT Condensed",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            50.0f,
            L"", //locale
            &g_pDWriteTextFormat
        );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        g_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        g_pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr))
    {
        // WIC Factory 생성하고 인터페이스 얻기
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&g_pIWICImagingFactory)
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateD2DBitmapFromFile(L"..\\Resource\\atk.png", &g_pID2DBitmap);
    }

    if (FAILED(hr))
    {
        _com_error err(hr);
        MessageBox(g_hWnd, err.ErrorMessage(), L"FAILED", MB_OK);
    }
    return true;
}

void D2DRenderer::Uninitialize()
{
    if (g_pID2DBitmap != NULL) g_pID2DBitmap->Release();
    // WIC해제 
    if (g_pIWICImagingFactory != NULL) g_pIWICImagingFactory->Release();
    if (g_pDWriteTextFormat != NULL) g_pDWriteTextFormat->Release();
    if (g_pDWriteFactory != NULL) g_pDWriteFactory->Release();

    if (g_pBlackBrush != NULL) g_pBlackBrush->Release();
    if (g_pGrayBrush != NULL) g_pGrayBrush->Release();

    // 해제
    if (g_pD2DRenderTarget != NULL) g_pD2DRenderTarget->Release();
    // 해제
    if (g_pD2DFactory != NULL) g_pD2DFactory->Release();
    //COM사용끝
    CoUninitialize();
}

HRESULT D2DRenderer::CreateD2DBitmapFromFile(const WCHAR* szFilePath, ID2D1Bitmap** pID2D1Bitmap)
{
    HRESULT hr;
    // Create a decoder
    IWICBitmapDecoder* pDecoder = NULL;

    // 비트맵 디코더 생성
    hr = g_pIWICImagingFactory->CreateDecoderFromFilename(
        szFilePath,                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    IWICFormatConverter* pConverter = NULL;
    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        hr = g_pIWICImagingFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = g_pD2DRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, pID2D1Bitmap);
    }

    if (pFrame != NULL)
        pFrame->Release();

    if (pConverter != NULL)
        pConverter->Release();

    if (pDecoder != NULL)
        pDecoder->Release();

    return hr;
}

bool D2DRenderer::TestScene()
{
    g_pD2DRenderTarget->BeginDraw();

    D2D1::ColorF color(D2D1::ColorF::Red);

    g_pD2DRenderTarget->Clear(color);

    D2D1_RECT_F rect;
    rect.left = 300;
    rect.top = 300;
    rect.right = rect.left + 300;
    rect.bottom = rect.top + 300;

    g_pD2DRenderTarget->FillRectangle(rect, g_pGrayBrush);

    rect.left = 400;
    rect.top = 400;
    rect.right = rect.left + 300;
    rect.bottom = rect.top + 300;

    g_pD2DRenderTarget->DrawRectangle(rect, g_pBlackBrush);

    static const WCHAR sc_helloWorld[] = L"Hello, World!";
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = g_pD2DRenderTarget->GetSize();

    g_pD2DRenderTarget->DrawText(
        sc_helloWorld,
        ARRAYSIZE(sc_helloWorld) - 1,
        g_pDWriteTextFormat,
        D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
        g_pBlackBrush
    );

    // 0,0
    g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap);


    // 200,200
    g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    D2D1_SIZE_F size = g_pID2DBitmap->GetSize();
    rect.left = 200;
    rect.top = 200;
    rect.right = rect.left + size.width * 1.5;
    rect.bottom = rect.left + size.height * 1.5;
    g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap, rect);


    // Transform만 적용한다.
    g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0, 300));
    g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap);


    // 그리기 끝
    g_pD2DRenderTarget->EndDraw();

    return false;
}

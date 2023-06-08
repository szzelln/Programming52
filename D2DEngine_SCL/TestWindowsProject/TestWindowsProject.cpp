// TestWindowsProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "TestWindowsProject.h"

#include "../TestStaticLib/MyClass.h"
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dwrite.h>

//코덱 추가
#include <wincodec.h>
#include <comdef.h>


#pragma comment(lib, "windowscodecs.lib")


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 기본 D2D사용을 위한 Factory의 인터페이스 포인터
ID2D1Factory* g_pD2DFactory = NULL;
ID2D1HwndRenderTarget* g_pD2DRenderTarget = NULL;

// 렌더타겟에서생성한 브러시의 인터페이스 포인터
ID2D1SolidColorBrush* g_pGrayBrush = NULL;
ID2D1SolidColorBrush* g_pBlackBrush = NULL;

// 텍스트 출력을 위한 객체 인터페이스 포인터
IDWriteFactory* g_pDWriteFactory = NULL;
IDWriteTextFormat* g_pDWriteTextFormat = NULL;

//WIC 인터페이스
IWICImagingFactory* g_pIWICImagingFactory = NULL;

//이미지 로드 후 생성하고 받아오는 인터페이스
ID2D1Bitmap* g_pID2DBitmap = NULL;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//파일(비트맵 등) 가져오는 함수
HRESULT CreateD2DBitmapFromFile(const WCHAR* szFilePath, ID2D1Bitmap** pID2D1Bitmap)
{
    HRESULT hr;

    // Create a decoder
    IWICBitmapDecoder* pDecoder = NULL;

    //WIC용 Factory객체를 사용하여 이미지 압축 해제를 위한 객체를 생성
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
        g_pD2DRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, pID2D1Bitmap);
    }


    if (pFrame != NULL)
    {
        pFrame->Release();
    }

    if (pConverter != NULL)
    {
        pConverter->Release();
    }

    if (pDecoder != NULL)
    {
        pDecoder->Release();
    }


    return hr;

}

bool InitD2D()
{

    HRESULT hr;

    // COM사용
    hr = CoInitialize(NULL);

    // 렌더타겟을 만들수있는팩토리객체를 생성하고 인터페이스 포인터를얻어온다.
    if (SUCCEEDED(hr))
    {
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

    // Create WIC factory
    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&g_pIWICImagingFactory)
        );
    }

    if (SUCCEEDED(hr))
    {
        CreateD2DBitmapFromFile(L"..\\Resource\\atk.png", &g_pID2DBitmap); //L"" => 경로
    }

    // 파일 불러오기 실패하면 오류 메세지 박스 뜸
    if (FAILED(hr))
    {
        _com_error err(hr);
        ::MessageBox(g_hWnd, err.ErrorMessage(), L"FAILED", MB_OK);
    }

    return true;
}

void UninitializeD2D()
{
    if (g_pID2DBitmap != NULL) g_pID2DBitmap->Release();

    //WIC해제
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


//메인함수 - 여러가지 인자를 받아오면서 실행
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTWINDOWSPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    InitD2D();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTWINDOWSPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 그리기 시작 알린다.
            g_pD2DRenderTarget->BeginDraw();

            D2D1::ColorF color(D2D1::ColorF::SpringGreen);

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

            //(0,0) 에다가 그리는 것
            g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap);


            //위치 및 사이즈 변환
            g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            D2D1_SIZE_F size = g_pID2DBitmap->GetSize();
            rect.left = 500;
            rect.top = 500;
            rect.right = rect.left + size.width * 1.5;
            rect.bottom = rect.top + size.height * 1.5;
            g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap, rect);

            //Transform만 적용한다.
            g_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(100, 300));
            g_pD2DRenderTarget->DrawBitmap(g_pID2DBitmap);


            // 그리기 끝
            g_pD2DRenderTarget->EndDraw();
        }
    }
    UninitializeD2D();
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINDOWSPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_TESTWINDOWSPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

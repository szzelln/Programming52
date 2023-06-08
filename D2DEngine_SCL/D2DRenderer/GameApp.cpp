// GameApp : 기본 윈도우 생성, 메세지 처리

#include "pch.h"
#include "GameApp.h"
#include "D2DRenderer.h"

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

//메시지를 처리하기 위해 작성되는 콜백 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
/*
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    */
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


// == Windows에 ATOM MyRegisterClass(HINSTANCE hInstance) 용도: 창 클래스를 등록합니다.
GameApp::GameApp(HINSTANCE hInstance)
    :m_hInst(hInstance),m_szWindowClass(L"DefaultWindowClass"),m_szTitle(L"DefaultWindowTitle")
{ 
    //특정 게임App과 상관없는 기본 세팅
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hInstance = m_hInst;
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszMenuName = NULL;
    m_wcex.lpszClassName = m_szWindowClass;   
}

GameApp::~GameApp()
{
}

bool GameApp::Initialize() // == Windows에 BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // m_wcex 준비되었다고 가정
    RegisterClassExW(&m_wcex);
    
    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    m_pRenderer = new D2DRenderer(m_hWnd);
    return m_pRenderer->Initialize();

    return true;
}

void GameApp::Loop() // 기본 메시지 루프
{
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
            Update();
            Render();
        }
    }
}

void GameApp::Update()
{
}

void GameApp::Render()
{
}

void GameApp::Uninitialize()
{
    delete m_pRenderer; // D2DRenderer* m_pRenderer 를 파괴함
}

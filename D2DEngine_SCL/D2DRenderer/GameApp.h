#pragma once
// GameApp : 기본 윈도우 생성, 메세지 처리

#define MAX_LOADSTRING 100

class D2DRenderer;
class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Initialize();
	void Loop();
	virtual void Update();
	virtual void Render();
	void Uninitialize();

protected:
	D2DRenderer* m_pRenderer;
	WNDCLASSEXW m_wcex;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HACCEL m_hAccelTable;
	WCHAR m_szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
};


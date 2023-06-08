#pragma once
// GameApp : �⺻ ������ ����, �޼��� ó��

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
	WCHAR m_szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
};


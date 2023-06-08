// D2DRenderer : D2D관련 인터페이스

#include <d2d1_1helper.h>
#pragma comment(lib, "windowscodecs.lib")

#pragma once
class D2DRenderer
{
public:
	D2DRenderer(HWND hwnd);
	~D2DRenderer();


	bool Initialize();
	void Uninitialize();

	HRESULT CreateD2DBitmapFromFile(const WCHAR* szFilePath, ID2D1Bitmap** pID2D1Bitmap);

	bool TestScene();

private:
	HWND g_hWnd;

protected:

	// 기본 D2D사용을 위한 Factory의 인터페이스 포인터
	ID2D1Factory* g_pD2DFactory = NULL;
	ID2D1HwndRenderTarget* g_pD2DRenderTarget = NULL;

	// 렌더타겟에서생성한 브러시의 인터페이스 포인터
	ID2D1SolidColorBrush* g_pGrayBrush = NULL;
	ID2D1SolidColorBrush* g_pBlackBrush = NULL;

	// 텍스트 출력을 위한 객체 인터페이스 포인터
	IDWriteFactory* g_pDWriteFactory = NULL;
	IDWriteTextFormat* g_pDWriteTextFormat = NULL;

	// WIC 인터페이스
	IWICImagingFactory* g_pIWICImagingFactory = NULL;

	// 이미지로드후 생성하고 받아오는 인터페이스
	ID2D1Bitmap* g_pID2DBitmap = NULL;

};


// D2DRenderer : D2D���� �������̽�

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

	// �⺻ D2D����� ���� Factory�� �������̽� ������
	ID2D1Factory* g_pD2DFactory = NULL;
	ID2D1HwndRenderTarget* g_pD2DRenderTarget = NULL;

	// ����Ÿ�ٿ��������� �귯���� �������̽� ������
	ID2D1SolidColorBrush* g_pGrayBrush = NULL;
	ID2D1SolidColorBrush* g_pBlackBrush = NULL;

	// �ؽ�Ʈ ����� ���� ��ü �������̽� ������
	IDWriteFactory* g_pDWriteFactory = NULL;
	IDWriteTextFormat* g_pDWriteTextFormat = NULL;

	// WIC �������̽�
	IWICImagingFactory* g_pIWICImagingFactory = NULL;

	// �̹����ε��� �����ϰ� �޾ƿ��� �������̽�
	ID2D1Bitmap* g_pID2DBitmap = NULL;

};


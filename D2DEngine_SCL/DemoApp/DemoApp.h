// DemoApp : GameApp 클래스를 상속받음
#pragma once

#include "resource.h"
#include "../D2DRenderer/GameApp.h"

class DemoApp : public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	virtual void Update();	
	virtual void Render();
private:

};


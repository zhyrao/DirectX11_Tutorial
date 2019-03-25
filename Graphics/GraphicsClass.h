#pragma once

#include<windows.h>


//////////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////////
const bool         FULL_SCREEN       = false;
const bool         VSYNC_ENABLED     = true;
const float        SCREEN_DEPTH      = 1000.0f;
const float        SCREEN_NEAR       = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Initialize(int width, int height, HWND hWnd);
	void ShutDown();
	bool Frame();

private:
	bool Render();
};


#pragma once

#include<windows.h>

#include "input/InputClass.h"
#include "Graphics/GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& sc);
	~SystemClass();

	bool Initialize();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();


private:
	LPCWSTR             m_ApplicationName;
	HINSTANCE           m_hInstance;
	HWND                m_Hwnd;

	InputClass*         m_Input;
	GraphicsClass*      m_Graphics;
};


// FUNCTION PROTOTYPES
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

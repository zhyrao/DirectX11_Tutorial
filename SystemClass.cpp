#include "SystemClass.h"



SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}


SystemClass::SystemClass(const SystemClass& sc)
{

}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	if (!m_Input)
		return false;

	result = m_Input->Initialize(m_hInstance, m_Hwnd, screenWidth, screenHeight);
	if (!result) {
		MessageBox(m_Hwnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
		return false;

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_Hwnd);
	if (!result)return false;

	return true;
}

void SystemClass::ShutDown()
{
	// release graphics object
	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// release input object
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// shutdown the window
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				MessageBox(m_Hwnd, "Frame Processing Failed", "Error", MB_OK);
				done = true;
			}
		}
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, umsg, wParam, lParam);
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;

	// do the input frame processing
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// get the location of the mouse from the input object
	m_Input->GetMouseLocation(mouseX, mouseY);

	result = m_Graphics->Frame(mouseX, mouseY);
	if (!result)
		return false;

	// finally render the graphics to the screen
	result = m_Graphics->Render();
	if (!result) return false;

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	m_ApplicationName = L"DirectX 11 Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPSTR)m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_Hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, (LPSTR)m_ApplicationName, (LPSTR)m_ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL,
		NULL, m_hInstance, NULL);

	ShowWindow(m_Hwnd, SW_SHOW);
	SetForegroundWindow(m_Hwnd);
	SetFocus(m_Hwnd);

	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_Hwnd);
	m_Hwnd = NULL;

	UnregisterClass((LPSTR)m_ApplicationName, m_hInstance);
	m_hInstance = NULL;

	ApplicationHandle = NULL;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

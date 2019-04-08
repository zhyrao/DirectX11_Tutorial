#pragma once

// preprocessing directives
#define  DIRECTINPUT_VERSION 0x0800


// LINKING
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

// include
#include<dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	// initialize all keys state
	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenW, int screenH);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& posx, int& posy);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};


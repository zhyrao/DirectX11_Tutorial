#include "InputClass.h"
#include <assert.h>



InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
}

// init all keys state to released and not pressed
void InputClass::Initialize() {
	for (int i = 0;	i < 256; i++)
	{
		m_keys[i] = false;
	}
}

// key pressed
void InputClass::KeyDown(unsigned int keyCode)
{
	//assert(keyCode < 0 || keyCode >= 256);
	m_keys[keyCode] = true;
}

// key released
void InputClass::KeyUp(unsigned int keyCode)
{
	//assert(keyCode < 0 || keyCode >= 256);
	m_keys[keyCode] = false;
}

bool InputClass::IsKeyDown(unsigned int keyCode)
{
	//assert(keyCode < 0 || keyCode >= 256);
	return m_keys[keyCode];
}
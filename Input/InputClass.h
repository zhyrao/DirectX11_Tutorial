#pragma once
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	// initialize all keys state
	void Initialize();

	// key down and up
	void KeyDown(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);


	// get a key state
	bool IsKeyDown(unsigned int keyCode);

private:
	// key state holder
	bool m_keys[256];
};


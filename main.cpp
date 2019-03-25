#include "SystemClass.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SystemClass* system;
	bool result;

	system = new SystemClass();

	if (!system)
		return 0;

	// initialize
	result = system->Initialize();
	// main loop
	if (result)
		system->Run();

	system->ShutDown();
	delete system;
	system = 0;

	return 0;
}
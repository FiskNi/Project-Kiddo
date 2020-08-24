#include "GameEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Debug console
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout); //Redirects the string stream to the debug console
	cout << "Welcome to the debug console!" << endl;

	// The Game
	GameEngine gProjectKiddo;
	gProjectKiddo.Run();

	return 0;
}


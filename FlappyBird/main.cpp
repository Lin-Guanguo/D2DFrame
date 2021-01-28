#include "pch.h"
#include "GameWindow.h"

using namespace LGG;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	THROW_ON_FAILED(
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)
	);

	GameWindow win;
	win.showWindow(nCmdShow);
	win.run();

}
#include "pch.h"
#include "GameWindow.h"
#include "Util.h"

using namespace LGG;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
	try {
		THROW_ON_FAILED(
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)
		);

		GameWindow win;
		win.showWindow(nCmdShow);
		win.run();
	}
	catch (Direct2DError& e){
		DebugOutPut(e.hresult(), L" ", e.what());
	}

}
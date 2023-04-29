#include "CCore.h"

using namespace std;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	CCore window(L"firstWindow", hInstance);

	if (FAILED(window.Create()))
	{
		return -1;
	}

	return window.Run();
}
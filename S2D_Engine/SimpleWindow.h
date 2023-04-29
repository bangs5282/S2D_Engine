#ifndef _SimpleWindow
#define _SimpleWindow

#pragma comment(linker, "/subsystem:windows")	 /* 윈도우 창을 띄울 수 있게 해줍니다. */

#include <windows.h>
#include <memory>	   // std::unique_ptr

#include <timeapi.h>
#pragma comment(lib, "Winmm.lib")

#include <thread>

//#include<gdiplusgraphics.h>
#include <Gdiplus.h>             // GDI+를 사용하기 위한 헤더 파일 
//#pragma comment(lib, "gdiplus")  // GDI+를 사용하기 위한 라이브러리 파일 
#include <assert.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

/* 윈도우 정보 */
struct WindowInfo
{
	DWORD WinStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL;
	INT	  WinX = 20;
	INT	  WinY = 10;
	INT	  WinWidth = 800;
	INT	  WinHeight = 600;
};

class SimpleWindow final
{
public:
	SimpleWindow(LPCWSTR name, HINSTANCE hInst = GetModuleHandle(NULL));

	INT	 Create();	/* 윈도우 클래스를 생성 및 등록 & DX 초기화 */
	INT	 Run();		/* 게임 루프 실행 */

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* Wrapper(WndProc에서 호출) */
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* 메세지 처리 루프 */
	static void			   CleanUp(); /* 데이터 소멸 */

	static std::unique_ptr<WindowInfo> mWinInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst;	/* 프로그램 핸들 */
	HWND		mhWnd;  /* 윈도우 핸들 */
	HDC mHdc;
	//Gdiplus::Graphics mGph;

};

#endif
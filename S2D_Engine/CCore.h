#ifndef _CCore
#define _CCore

#include <memory>
#include <windows.h>

#include "CObject.h"
#include "TimeMgr.h"

/* 윈도우 정보 */
struct WindowInfo
{
	DWORD WinStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL;
	INT	  WinX = 20;
	INT	  WinY = 10;
	INT	  WinWidth = 800;
	INT	  WinHeight = 600;
};

class CCore final
{
public:
	CCore(LPCWSTR name, HINSTANCE hInst = GetModuleHandle(NULL));

	INT	 Create();	/* 윈도우 클래스를 생성 및 등록 & DX 초기화 */
	INT	 Run();		/* 게임 루프 실행 */

	void Progress();
	void Update();
	void Render();

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* Wrapper(WndProc에서 호출) */
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* 메세지 처리 루프 */
	static void			   CleanUp(HWND hWnd, HDC hDC); /* 데이터 소멸 */

	static std::unique_ptr<WindowInfo> mWinInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst;	/* 프로그램 핸들 */
	HWND		mhWnd;  /* 윈도우 핸들 */
	HDC			mhDC;
	//Gdiplus::Graphics mGph;

	CObject mPt;
};



#endif
#ifndef _CCore
#define _CCore

#include <memory>
#include <windows.h>

#include "CObject.h"
#include "TimeMgr.h"

/* ������ ���� */
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

	INT	 Create();	/* ������ Ŭ������ ���� �� ��� & DX �ʱ�ȭ */
	INT	 Run();		/* ���� ���� ���� */

	void Progress();
	void Update();
	void Render();

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* Wrapper(WndProc���� ȣ��) */
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* �޼��� ó�� ���� */
	static void			   CleanUp(HWND hWnd, HDC hDC); /* ������ �Ҹ� */

	static std::unique_ptr<WindowInfo> mWinInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst;	/* ���α׷� �ڵ� */
	HWND		mhWnd;  /* ������ �ڵ� */
	HDC			mhDC;
	//Gdiplus::Graphics mGph;

	CObject mPt;
};



#endif
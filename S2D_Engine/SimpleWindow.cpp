#include "SimpleWindow.h"

/* static ��� ���� �ʱ�ȭ */
std::unique_ptr<WindowInfo> SimpleWindow::mWinInfo = std::make_unique<WindowInfo>();

/* �����ڿ��� ���α׷� �ڵ��� ����ϴ�. */
SimpleWindow::SimpleWindow(LPCWSTR name, HINSTANCE hInst)
	: mName(name)
	, mhInst(hInst)
	, mhWnd()
{
}

LRESULT SimpleWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		}

		return 0;
	}

	case WM_DESTROY:
	{
		CleanUp();
		PostQuitMessage(0); /* �޼���ť�� WM_QUIT �޼��� ���� */
		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); /* ���α׷��� ó������ ���� ����� �޼����� ó�� */
}

LRESULT WINAPI SimpleWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

INT SimpleWindow::Create()
{
	/* ������ Ŭ������ ����մϴ�. */
	WNDCLASS wc =
	{
		  CS_CLASSDC
		, WndProc		/* �޼��� ó�� �Լ��� WndProc�� ������־�� �� */
		, 0L
		, 0L
		, mhInst		/* WinMain���κ��� ���� ���α׷� �ڵ�(�ν��Ͻ�) */
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, mName		    /* ������ Ŭ���� �̸� */
	};

	RegisterClass(&wc);

	/* ������ ������ �����ݴϴ�. ������ ũ�� �Ű�X */
	RECT rc;

	SetRect(&rc, 0, 0, 500, 300);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	/* �����츦 �����մϴ�. */
	mhWnd = CreateWindow
	(
		mName					/* WNDCLASS Ÿ�� ��ü�� ������ Ŭ���� �̸��� ���� */
		, mName					/* ������ Ÿ��Ʋ �ٿ� ��� �̸� */
		, mWinInfo->WinStyle
		, 20					/*   x	  */
		, 10					/*   y	  */
		, mWinInfo->WinWidth	/* width  */
		, mWinInfo->WinHeight	/* height */
		, GetDesktopWindow()	/* �θ� �Ǵ� ������ �������� �ڵ� ���� */
		, NULL
		, mhInst
		, NULL
	);

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	ShowCursor(TRUE);

	/*
		***********************
		���⿡ DX�� �ʱ�ȭ�մϴ�.
		***********************
	*/

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	assert(st == Ok);
	if (st != Ok) return FALSE;

	mHdc = GetDC(mhWnd);
	//mGph.FromHWND(mhWnd);

	return 0;
}

void test(HWND mhWnd)
{
	DWORD startTime = timeGetTime();                  //������ �ð�(temp����)
	int frameCount = 0;

	float timeElapsed;




	while (1) {
		DWORD curTime = timeGetTime();                          //���� �ð�

		timeElapsed = (float)(curTime - startTime) / 1000;
		if (timeElapsed >= 1.0f)                                //�帥�ð��� 1���̻��̸� ���� �ϰ������ ó��
		{
			/*
			WCHAR pMessage[300] = { 0, };
			float fps = (float)frameCount / timeElapsed;
			swprintf_s(pMessage, sizeof(pMessage), TEXT("FPS %f"), fps);
			//OutputDebugStringW(pMessage);
			*/
			float fps = (float)frameCount / timeElapsed;

			float vIn = fps;
			char vOutChar[100];
			_gcvt_s(vOutChar, sizeof(vOutChar), vIn, 8);
			wchar_t vOut[100];
			mbstowcs_s(NULL, vOut, sizeof(vOut) / 2, vOutChar, sizeof(vOutChar));



			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(mhWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			Graphics gr(mhWnd);
			FontFamily temp(L"Arial");
			Font font(&temp, 12);
			LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Yellow, LinearGradientModeHorizontal);
			Status st = gr.DrawString(vOut, -1, &font, PointF(0, 0), &brush);
			assert(st == Ok);
			EndPaint(mhWnd, &ps);



			frameCount = 0;
			startTime = timeGetTime();


		}

		frameCount++;
	}
}

INT SimpleWindow::Run()
{
	//std::thread t1(test, mhWnd);
	//t1.detach();

	// �޼��� ������ �����մϴ�.
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*
				**************
				���� ���� �ۼ�
				**************
			*/
		}
	}

	UnregisterClass(mName, mhInst);

	return 0;
}

void SimpleWindow::CleanUp()
{
	/*
		***********************
		 ���� ������ + DX ����
		***********************
	*/
}
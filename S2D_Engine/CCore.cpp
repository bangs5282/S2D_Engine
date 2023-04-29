#include "CCore.h"

/* static ��� ���� �ʱ�ȭ */
std::unique_ptr<WindowInfo> CCore::mWinInfo = std::make_unique<WindowInfo>();

/* �����ڿ��� ���α׷� �ڵ��� ����ϴ�. */
CCore::CCore(LPCWSTR name, HINSTANCE hInst)
	: mName(name)
	, mhInst(hInst)
	, mhWnd()
{
}

LRESULT CCore::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		CleanUp(hWnd, GetDC(hWnd));
		PostQuitMessage(0); /* �޼���ť�� WM_QUIT �޼��� ���� */
		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); /* ���α׷��� ó������ ���� ����� �޼����� ó�� */
}

LRESULT WINAPI CCore::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

INT CCore::Create()
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
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
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

	/*
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	assert(st == Ok);
	if (st != Ok) return FALSE;
	*/

	mhDC = GetDC(mhWnd);
	//mGph.FromHWND(mhWnd);
	TimeMgr::GetInst()->init();
	
	mPt.setPos(Vec2{ 110, 110 });
	mPt.setScale(Vec2(100 , 100));

	return 0;
}

/*
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
			/
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
*/

INT CCore::Run()
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
			Progress();
		}
	}

	UnregisterClass(mName, mhInst);

	return 0;
}

void CCore::Progress() {
	Update();
	Render();

	TimeMgr::GetInst()->update();
}

void CCore::Update() {
	
	Vec2 vPos = mPt.getPos();

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		vPos.x -= 100.f * TimeMgr::GetInst()->GetDT();
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		vPos.x += 100.f * TimeMgr::GetInst()->GetDT();
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		vPos.y -= 100.f * TimeMgr::GetInst()->GetDT();
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		vPos.y += 100.f * TimeMgr::GetInst()->GetDT();
	}
	
	mPt.setPos(vPos);
}

void CCore::Render() {
	
	Vec2 vPos = mPt.getPos();
	Vec2 vSale = mPt.getScale();

	Rectangle(mhDC, int(vPos.x - vSale.x / 2.f)
					, int(vPos.y - vSale.y / 2.f)
					, int(vPos.x + vSale.x / 2.f)
					, int(vPos.y + vSale.y / 2.f));
	
}

void CCore::CleanUp(HWND hWnd, HDC hDC)
{
	/*
		***********************
		 ���� ������ + DX ����
		***********************
	*/
	ReleaseDC(hWnd, hDC);
}
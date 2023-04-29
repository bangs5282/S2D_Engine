#include "CCore.h"

/* static 멤버 변수 초기화 */
std::unique_ptr<WindowInfo> CCore::mWinInfo = std::make_unique<WindowInfo>();

/* 생성자에서 프로그램 핸들을 얻습니다. */
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
		PostQuitMessage(0); /* 메세지큐에 WM_QUIT 메세지 전달 */
		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); /* 프로그램이 처리하지 않은 잡다한 메세지를 처리 */
}

LRESULT WINAPI CCore::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

INT CCore::Create()
{
	/* 윈도우 클래스를 등록합니다. */
	WNDCLASS wc =
	{
		  CS_CLASSDC
		, WndProc		/* 메세지 처리 함수인 WndProc을 등록해주어야 함 */
		, 0L
		, 0L
		, mhInst		/* WinMain으로부터 받은 프로그램 핸들(인스턴스) */
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, mName		    /* 윈도우 클래스 이름 */
	};

	RegisterClass(&wc);

	/* 윈도우 영역을 정해줍니다. 지금은 크게 신경X */
	RECT rc;

	SetRect(&rc, 0, 0, 500, 300);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	/* 윈도우를 생성합니다. */
	mhWnd = CreateWindow
	(
		mName					/* WNDCLASS 타입 객체의 윈도우 클래스 이름과 동일 */
		, mName					/* 윈도우 타이틀 바에 띄울 이름 */
		, mWinInfo->WinStyle
		, 20					/*   x	  */
		, 10					/*   y	  */
		, mWinInfo->WinWidth	/* width  */
		, mWinInfo->WinHeight	/* height */
		, GetDesktopWindow()	/* 부모 또는 소유주 윈도우의 핸들 지정 */
		, NULL
		, mhInst
		, NULL
	);

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	ShowCursor(TRUE);

	/*
		***********************
		여기에 DX를 초기화합니다.
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
	DWORD startTime = timeGetTime();                  //마지막 시간(temp변수)
	int frameCount = 0;

	float timeElapsed;




	while (1) {
		DWORD curTime = timeGetTime();                          //현재 시간

		timeElapsed = (float)(curTime - startTime) / 1000;
		if (timeElapsed >= 1.0f)                                //흐른시간이 1초이상이면 내가 하고싶은것 처리
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

	// 메세지 루프로 진입합니다.
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
		 게임 데이터 + DX 해제
		***********************
	*/
	ReleaseDC(hWnd, hDC);
}
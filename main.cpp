//--------------------------------------------------------------------------------
//
//　キーフレーム基盤オブジェクト
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "input.h"

#if defined(_DEBUG) || defined(EDITOR)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "ImGui\imgui_impl_dx9.h"
#endif
#endif

//--------------------------------------------------------------------------------
//	extern関数
//--------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(EDITOR)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#endif

//--------------------------------------------------------------------------------
//	メイン関数
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int cmdShow)
{
	return Main::WinMain(hInstance, hPrevInstance, IpCmdLine, cmdShow);
}

//--------------------------------------------------------------------------------
//	クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：WinMain
//  関数説明：メイン関数
//	引数：	hInstance
//			hPrevInstance
//			IpCmdLine
//			nCmdShow
//	戻り値：int
//--------------------------------------------------------------------------------
int Main::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(IpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEXのメモリサイズを指定
	wcex.style = CS_CLASSDC;							//表示するウインドウのスタイルを設定
	wcex.lpfnWndProc = wndProc;							//関数ポインタ、ウインドウプロシージャのアドレス（関数名）を指定
	wcex.cbClsExtra = 0;								//通常は使用しないので０を指定
	wcex.cbWndExtra = 0;								//通常は使用しないので０を指定
	wcex.hInstance = hInstance;							//WinMainのパラメータのインスタンスハンドル
	wcex.hIcon = NULL;									//自作icon出す、使用するアイコンを指定（Windowsがもっている）
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//マウスカーソルを指定
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//ウインドウのクライアント領域の背景色を設定
	wcex.lpszMenuName = NULL;							//Windowにつけるメニューを設定（つけると重い）
	wcex.lpszClassName = CLASS_NAME;					//ウインドウクラスの名前
	wcex.hIconSm = NULL;								//拡張された部分（ミニicon）、小さいアイコンが設定された場合の情報を記述
	RegisterClassEx(&wcex);								//ウインドウクラスの登録

	HWND hWnd;
	MSG msg;
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT cr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT dr;

	//window size
	AdjustWindowRect(&cr, style, false);
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//window pos
	GetWindowRect(GetDesktopWindow(), &dr);
	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	//画面溢出防止
	int wx = ww > dw ? 0 : (dw - ww) / 2;
	int wy = wh > dh ? 0 : (dh - wh) / 2;

	//ウインドウを作成
	hWnd = CreateWindowEx(
		0,				//拡張ウインドウスタイル
		CLASS_NAME,		//クラスの名前
		WINDOW_NAME,	//ウインドウの名前
		style,			//**important**window type,ウインドウのスタイル
		wx,				//ウインドウ左上座標X
		wy,				//ウインドウ左上座標Y
		ww,				//幅（ウインドウ全体）
		wh,				//高さ（ウインドウ全体）
		NULL,			//親ウィンドウのハンドル
		NULL,			//メニューハンドルまたは子ウインドウID
		hInstance,		//インスタンスハンドル
		NULL);			//ウインドウ作成データ

	//フルスクリーン
#ifdef _DEBUG
	bool isWindowMode = true;
#else
	bool isWindowMode = true;
	UINT id = MessageBox(hWnd, "フルスクリーンモードで起動しますか？", "確認", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
	if (id == IDYES) { isWindowMode = false; }
#endif // _DEBUG

	//Manager生成
	if (!Manager::Create(hInstance, hWnd, isWindowMode))
	{
		UnregisterClass(CLASS_NAME, wcex.hInstance);
		return -1;
	}

	//ウインドウの表示
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	//時間カウンタ
	LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER execLastTime;
	LARGE_INTEGER FPSLastTime;
	memset(&frequency, 0x00, sizeof frequency);
	memset(&currentTime, 0x00, sizeof currentTime);
	memset(&execLastTime, 0x00, sizeof execLastTime);
	memset(&FPSLastTime, 0x00, sizeof FPSLastTime);
	QueryPerformanceCounter(&execLastTime);
	FPSLastTime = execLastTime;

	//メッセージループ
	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//window処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&currentTime);
			float fTime = (float)(currentTime.QuadPart - execLastTime.QuadPart) * 1000.0f / (float)frequency.QuadPart;
			
			if (fTime >= TIMER_INTERVAL)
			{
				execLastTime = currentTime;
				Manager::Instance()->Update();
				Manager::Instance()->LateUpdate();
				Manager::Instance()->Render();
			}
		}
	}

	// 終了処理
	Manager::Release();

	//ウインドウクラスの登録お解除
	//第一引数：メクラス名
	//第二引数：アプリケーションインスタン
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------
//	関数名：wndProc
//  関数説明：ウインドウプロシージャ関数
//	引数：	hWnd：ウインドウのハンドル
//			uMsg：メッセージの識別子
//			wParam：メッセージの最初のパラメータ
//			lParam：メッセージの二番目のパラメータ
//	戻り値：LRESULT
//--------------------------------------------------------------------------------
LRESULT CALLBACK Main::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#if defined(_DEBUG) || defined(EDITOR)
	ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam);
#endif // _DEBUG

	switch (uMsg) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wParam) == VK_ESCAPE) { closeApp(hWnd); }
		break;
	case WM_CLOSE:
		closeApp(hWnd);
		return 0;
	case WM_DESTROY:		//保存するかチェックの仕組みを作れる
		PostQuitMessage(0);	//WM_QUITというメッセージを呼ぶ
		break;
	case WM_ACTIVATEAPP:
		if (Input::Instance())
		{
			BOOL isActive = (BOOL)GetActiveWindow();
			if (isActive)
			{
				Input::Instance()->Acquire();
			}
			else
			{
				Input::Instance()->Unacquire();
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------------------------
//	関数名：closeApp
//  関数説明：アプリを閉じる確認関数
//	引数：	hWnd：ウインドウのハンドル
//	戻り値：LRESULT
//--------------------------------------------------------------------------------
void Main::closeApp(HWND hWnd)
{
	//終了確認
	UINT id = MessageBox(hWnd, "終了しますか？", "確認", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//押し判定
	if (id == IDYES) {
		//WM_DESTROYメッセージを送信
		DestroyWindow(hWnd);
	}
}
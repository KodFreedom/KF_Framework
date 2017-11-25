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
//  定数定義
//--------------------------------------------------------------------------------
#define CLASS_NAME			L"KF_Framework"
#define WINDOW_NAME			L"フレームワーク"

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
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show)
{
	return Main::WinMain(instance, previous_instance, cmd_line, cmd_show);
}

//--------------------------------------------------------------------------------
// メイン関数
//--------------------------------------------------------------------------------
int Main::WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show)
{
	UNREFERENCED_PARAMETER(previous_instance);
	UNREFERENCED_PARAMETER(cmd_line);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEXのメモリサイズを指定
	wcex.style = CS_CLASSDC;							//表示するウインドウのスタイルを設定
	wcex.lpfnWndProc = WndProc;							//関数ポインタ、ウインドウプロシージャのアドレス（関数名）を指定
	wcex.cbClsExtra = 0;								//通常は使用しないので０を指定
	wcex.cbWndExtra = 0;								//通常は使用しないので０を指定
	wcex.hInstance = instance;							//WinMainのパラメータのインスタンスハンドル
	wcex.hIcon = NULL;									//自作icon出す、使用するアイコンを指定（Windowsがもっている）
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//マウスカーソルを指定
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//ウインドウのクライアント領域の背景色を設定
	wcex.lpszMenuName = NULL;							//Windowにつけるメニューを設定（つけると重い）
	wcex.lpszClassName = CLASS_NAME;					//ウインドウクラスの名前
	wcex.hIconSm = NULL;								//拡張された部分（ミニicon）、小さいアイコンが設定された場合の情報を記述
	RegisterClassEx(&wcex);								//ウインドウクラスの登録

	HWND hwnd;
	MSG message;
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT real_rect;

	//window size
	AdjustWindowRect(&rect, style, false);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//window pos
	GetWindowRect(GetDesktopWindow(), &real_rect);
	int real_width = real_rect.right - real_rect.left;
	int real_height = real_rect.bottom - real_rect.top;

	//画面溢出防止
	int window_x = width > real_width ? 0 : (real_width - width) / 2;
	int window_y = height > real_height ? 0 : (real_height - height) / 2;

	//ウインドウを作成
	hwnd = CreateWindowEx(
		0,				//拡張ウインドウスタイル
		CLASS_NAME,		//クラスの名前
		WINDOW_NAME,	//ウインドウの名前
		style,			//**important**window type,ウインドウのスタイル
		window_x,		//ウインドウ左上座標X
		window_y,		//ウインドウ左上座標Y
		width,			//幅（ウインドウ全体）
		height,			//高さ（ウインドウ全体）
		NULL,			//親ウィンドウのハンドル
		NULL,			//メニューハンドルまたは子ウインドウID
		instance,		//インスタンスハンドル
		NULL);			//ウインドウ作成データ

	//フルスクリーン
#ifdef _DEBUG
	bool is_window_mode = true;
#else
	bool is_window_mode = true;
	UINT id = MessageBox(hWnd, "フルスクリーンモードで起動しますか？", "確認", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
	if (id == IDYES) { is_window_mode = false; }
#endif // _DEBUG

	//Manager生成
	if (!Manager::Create(instance, hwnd, is_window_mode))
	{
		UnregisterClass(CLASS_NAME, wcex.hInstance);
		return -1;
	}

	//ウインドウの表示
	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd);

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
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//window処理
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&message);		//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&message);		//ウインドウプロシージャへメッセージを送出
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

	return (int)message.wParam;
}

//--------------------------------------------------------------------------------
//	ウインドウプロシージャ関数
//--------------------------------------------------------------------------------
LRESULT CALLBACK Main::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
#if defined(_DEBUG) || defined(EDITOR)
	ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam);
#endif // _DEBUG

	switch (message) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wparam) == VK_ESCAPE) { CloseApp(hwnd); }
		break;
	case WM_CLOSE:
		CloseApp(hwnd);
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

	return DefWindowProc(hwnd, message, wparam, lparam);
}

//--------------------------------------------------------------------------------
//	アプリを閉じる確認関数
//--------------------------------------------------------------------------------
void Main::CloseApp(HWND hwnd)
{
	//終了確認
	UINT id = MessageBox(hwnd, L"終了しますか？", L"確認", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//押し判定
	if (id == IDYES) {
		//WM_DESTROYメッセージを送信
		DestroyWindow(hwnd);
	}
}
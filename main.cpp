//--------------------------------------------------------------------------------
//
//　キーフレーム基盤オブジェクト
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				CloseApp(HWND hWnd);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
CManager*	g_pManager = NULL;	//Game Manager

#ifdef _DEBUG
int			g_nCntFPS = 0;		//FPSカウンタ
#endif

//--------------------------------------------------------------------------------
//  main関数
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(IpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEXのメモリサイズを指定
	wcex.style = CS_CLASSDC;							//表示するウインドウのスタイルを設定
	wcex.lpfnWndProc = WndProc;							//関数ポインタ、ウインドウプロシージャのアドレス（関数名）を指定
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

	//Manager生成
	g_pManager = new CManager;

	if (g_pManager->Init(hInstance, hWnd, true) == KF_FAILED)
	{
		return -1;
	};

	//FPS
	DWORD dwFrameCount;
	DWORD dwCurrentTime;//現時間
	DWORD dwExecLastTime;//実行終了時時間
	DWORD dwFPSLastTime;

	//分解能を設定
	timeBeginPeriod(1);

	//各カウンター初期化
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();//システム時刻をミリ秒単位で取得

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= TIMER_INTERVAL)
			{
				dwExecLastTime = dwCurrentTime;//処理した時間

#ifdef _DEBUG
				if ((dwCurrentTime - dwFPSLastTime) >= 500)//0.5秒ごとに実行
				{
					g_nCntFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;
					dwFrameCount = 0;
				}
#endif//_DEBUG

				// 更新処理
				g_pManager->Update();
				g_pManager->LateUpdate();

				// 描画処理
				g_pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	if (g_pManager != NULL)
	{
		g_pManager->Uninit();
		delete g_pManager;
		g_pManager = NULL;
	}

	//ウインドウクラスの登録お解除
	//第一引数：メクラス名
	//第二引数：アプリケーションインスタン
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------
//  ウインドウプロシージャ関数
//  (CALLBACK : Win32API関数を呼び出す時の規約)
//  hWnd ; ウインドウのハンドル
//  uMsg : メッセージの識別子
//  wPara : メッセージの最初のパラメータ
//  iParam : メッセージの二番目のパラメータ
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:		//esp key
		if (LOWORD(wParam) == VK_ESCAPE) { CloseApp(hWnd); }
		break;
	case WM_CLOSE:
		CloseApp(hWnd);
		return 0;
	case WM_DESTROY:		//保存するかチェックの仕組みを作れる
		PostQuitMessage(0);	//WM_QUITというメッセージを呼ぶ
		break;
	case WM_ACTIVATEAPP:
		if (g_pManager)
		{
			BOOL bActive = (BOOL)GetActiveWindow();
			CInputManager* pInput = g_pManager->GetInputManager();

			if (bActive)
			{
				pInput->Acquire();
			}
			else
			{
				pInput->Unacquire();
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------------------------
//  close確認関数
//--------------------------------------------------------------------------------
void CloseApp(HWND hWnd)
{
	UINT nID = 0;//メッセージbox戻り値

	//終了確認
	nID = MessageBox(hWnd, "終了しますか？", "確認", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//押し判定
	if (nID == IDYES) {
		//WM_DESTROYメッセージを送信
		DestroyWindow(hWnd);
	}
}

//--------------------------------------------------------------------------------
//  Game Manager取得
//--------------------------------------------------------------------------------
CManager *GetManager(void)
{
	return g_pManager;
}

//--------------------------------------------------------------------------------
//  FPSカウンター取得
//--------------------------------------------------------------------------------
#ifdef _DEBUG
int GetCountFPS(void)
{
	return g_nCntFPS;
}
#endif
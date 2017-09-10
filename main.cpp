//--------------------------------------------------------------------------------
//
//　キーフレーム基盤オブジェクト
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数宣言
//--------------------------------------------------------------------------------
CManager*	CMain::m_pManager = nullptr;

#ifdef _DEBUG
unsigned int	CMain::m_unFPS = 0;
#endif

//--------------------------------------------------------------------------------
//	メイン関数
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	return CMain::Main(hInstance, hPrevInstance, IpCmdLine, nCmdShow);
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
int CMain::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
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

	//Manager生成
	m_pManager = new CManager;

	if (!m_pManager->Init(hInstance, hWnd, true))
	{
		return -1;
	};

	//FPS
#ifdef _DEBUG
	unsigned int unFrameCount = 0;
#endif
	DWORD dwCurrentTime;//現時間
	DWORD dwExecLastTime;//実行終了時時間
	DWORD dwFPSLastTime;

	//分解能を設定
	timeBeginPeriod(1);

	//各カウンター初期化
	dwCurrentTime = 0;
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
					m_unFPS = (unFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;
					unFrameCount = 0;
				}

				//Debug表示
				m_pManager->GetDebugManager()->DisplayAlways("FPS : " + to_string(m_unFPS) + '\n');
				unFrameCount++;
#endif//_DEBUG

				// 更新処理
				m_pManager->Update();
				m_pManager->LateUpdate();

				// 描画処理
				m_pManager->Draw();
			}
		}
	}

	// 終了処理
	if (m_pManager)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
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
//	関数名：wndProc
//  関数説明：ウインドウプロシージャ関数
//	引数：	hWnd：ウインドウのハンドル
//			uMsg：メッセージの識別子
//			wParam：メッセージの最初のパラメータ
//			lParam：メッセージの二番目のパラメータ
//	戻り値：LRESULT
//--------------------------------------------------------------------------------
LRESULT CALLBACK CMain::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
		if (m_pManager)
		{
			BOOL bActive = (BOOL)GetActiveWindow();
			auto pInput = m_pManager->GetInputManager();

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
//	関数名：closeApp
//  関数説明：アプリを閉じる確認関数
//	引数：	hWnd：ウインドウのハンドル
//	戻り値：LRESULT
//--------------------------------------------------------------------------------
void CMain::closeApp(HWND hWnd)
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
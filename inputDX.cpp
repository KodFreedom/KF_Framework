//--------------------------------------------------------------------------------
//	DirectX入力処理
//　inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "inputDX.h"

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------
#define	COUNT_WAIT_REPEAT	(20)	// リピート待ち時間

//--------------------------------------------------------------------------------
// 静的変数
//--------------------------------------------------------------------------------
LPDIRECTINPUT8	CInputDX::m_pDInput = NULL;	// DirectInputオブジェクト

//--------------------------------------------------------------------------------
//
// CInputDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// コンストラスタ
//--------------------------------------------------------------------------------
CInputDX::CInputDX()
	: m_pDIDevice(NULL)
{

}

//--------------------------------------------------------------------------------
// 入力処理の初期化処理
//--------------------------------------------------------------------------------
HRESULT CInputDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
	if (!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}
	else
	{
		hr = S_OK;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// 入力処理の終了処理
//--------------------------------------------------------------------------------
void CInputDX::Uninit(void)
{
	// デバイスオブジェクトの破棄
	if (m_pDIDevice)
	{
		m_pDIDevice->Unacquire();
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//--------------------------------------------------------------------------------
//
// CKeyboardDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// コンストラスタ
//--------------------------------------------------------------------------------
CKeyboardDX::CKeyboardDX()
{
	// 各ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//--------------------------------------------------------------------------------
// キーボードの初期化処理
//--------------------------------------------------------------------------------
HRESULT CKeyboardDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Inputデバイスが生成に失敗！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "キーボードがありません！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//--------------------------------------------------------------------------------
// キーボード入力処理の開放
//--------------------------------------------------------------------------------
void CKeyboardDX::Uninit(void)
{
	// 入力処理の開放
	CInputDX::Uninit();
}

//--------------------------------------------------------------------------------
// キーボードの更新処理
//--------------------------------------------------------------------------------
void CKeyboardDX::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[m_nNumKeyMax];

	if (!m_pDIDevice)
	{
		return;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);

	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < m_nNumKeyMax; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(プレス)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(トリガー)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リリース)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リピート)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ削除(トリガー)
//--------------------------------------------------------------------------------
void CKeyboardDX::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

//--------------------------------------------------------------------------------
// CMouseDXコンストラスタ
//--------------------------------------------------------------------------------
CMouseDX::CMouseDX()
{
	// 各ワークのクリア
	ZeroMemory(m_aMouseState, sizeof m_aMouseState);
	ZeroMemory(m_aMouseStateTrigger, sizeof m_aMouseStateTrigger);
	ZeroMemory(m_aMouseStateRelease, sizeof m_aMouseStateRelease);
	ZeroMemory(m_aMouseStateRepeat, sizeof m_aMouseStateRepeat);
	ZeroMemory(m_aMouseStateRepeatCnt, sizeof m_aMouseStateRepeatCnt);
}

//--------------------------------------------------------------------------------
// マウスの初期化処理
//--------------------------------------------------------------------------------
HRESULT CMouseDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Inputデバイスが生成に失敗！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	//// デバイスの設定    
	//DIPROPDWORD diprop;
	//diprop.diph.dwSize = sizeof(diprop);
	//diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	//diprop.diph.dwObj = 0;
	//diprop.diph.dwHow = DIPH_DEVICE;
	//diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	//m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//--------------------------------------------------------------------------------
// マウスの更新処理
//--------------------------------------------------------------------------------
void CMouseDX::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 dims;

	hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	
	if (SUCCEEDED(hr))
	{
		//Button更新
		for (int nCntButton = 0; nCntButton < m_nNumMBMax; nCntButton++)
		{
			m_aMouseStateTrigger[nCntButton] = (m_aMouseState[nCntButton] ^ dims.rgbButtons[nCntButton]) & dims.rgbButtons[nCntButton];
			m_aMouseStateRelease[nCntButton] = (m_aMouseState[nCntButton] ^ dims.rgbButtons[nCntButton]) & ~dims.rgbButtons[nCntButton];
			m_aMouseStateRepeat[nCntButton] = m_aMouseStateTrigger[nCntButton];

			if (dims.rgbButtons[nCntButton])
			{
				m_aMouseStateRepeatCnt[nCntButton]++;
				if (m_aMouseStateRepeatCnt[nCntButton] >= COUNT_WAIT_REPEAT)
				{
					m_aMouseStateRepeat[nCntButton] = dims.rgbButtons[nCntButton];
				}
			}
			else
			{
				m_aMouseStateRepeatCnt[nCntButton] = 0;
				m_aMouseStateRepeat[nCntButton] = 0;
			}

			m_aMouseState[nCntButton] = dims.rgbButtons[nCntButton];
		}

		//マウス位置更新
		m_lMouseX = dims.lX;
		m_lMouseY = dims.lY;
		m_lMouseZ = dims.lZ;
	}
	else if (hr == DIERR_INPUTLOST)
	{
		m_pDIDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(プレス)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMousePress(int nButton)
{
	return (m_aMouseState[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(トリガー)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseTrigger(int nButton)
{
	return (m_aMouseStateTrigger[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リリース)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRelease(int nButton)
{
	return (m_aMouseStateRelease[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リピート)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRepeat(int nButton)
{
	return (m_aMouseStateRepeat[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// マウスデータ削除(トリガー)
//--------------------------------------------------------------------------------
void CMouseDX::FlushMouseTrigger(int nButton)
{
	m_aMouseStateTrigger[nButton] = 0;
}

//--------------------------------------------------------------------------------
// マウスX軸取得
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisX(void)
{
	return m_lMouseX;
}

//--------------------------------------------------------------------------------
// マウスY軸取得
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisY(void)
{
	return m_lMouseY;
}

//--------------------------------------------------------------------------------
// マウスZ軸取得(Wheel)
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisZ(void)
{
	return m_lMouseZ;
}
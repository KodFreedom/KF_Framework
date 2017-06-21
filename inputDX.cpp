//--------------------------------------------------------------------------------
//	DirectX入力処理
//　inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "inputDX.h"

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
CKeyboardDX::CKeyboardDX() : CInputDX()
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
// キーボードの更新処理
//--------------------------------------------------------------------------------
void CKeyboardDX::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[sc_nNumKeyMax];

	if (!m_pDIDevice)
	{
		return;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);

	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < sc_nNumKeyMax; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= sc_nCntForRepeat)
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
//
// CMouseDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// コンストラスタ
//--------------------------------------------------------------------------------
CMouseDX::CMouseDX() : CInputDX()
{
	// 各ワークのクリア
	ZeroMemory(&m_mouseState, sizeof DIMOUSESTATE2);
	ZeroMemory(&m_mouseStateTrigger, sizeof DIMOUSESTATE2);
	ZeroMemory(&m_mouseStateRelease, sizeof DIMOUSESTATE2);
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

	// デバイスの設定    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

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

	if (!m_pDIDevice) { return; }

	hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	
	if (SUCCEEDED(hr))
	{
		//Button更新
		for (int nCntButton = 0; nCntButton < sizeof(dims.rgbButtons); nCntButton++)
		{
			m_mouseStateTrigger.rgbButtons[nCntButton] = (m_mouseState.rgbButtons[nCntButton] ^ dims.rgbButtons[nCntButton]) & dims.rgbButtons[nCntButton];
			m_mouseStateRelease.rgbButtons[nCntButton] = (m_mouseState.rgbButtons[nCntButton] ^ dims.rgbButtons[nCntButton]) & ~dims.rgbButtons[nCntButton];
		}

		//マウス位置更新
		m_mouseStateTrigger.lX = (m_mouseState.lX ^ dims.lX) & dims.lX;
		m_mouseStateTrigger.lY = (m_mouseState.lY ^ dims.lY) & dims.lY;
		m_mouseStateTrigger.lZ = (m_mouseState.lZ ^ dims.lZ) & dims.lZ;
		m_mouseStateRelease.lX = (m_mouseState.lX ^ dims.lX) & ~dims.lX;
		m_mouseStateRelease.lY = (m_mouseState.lY ^ dims.lY) & ~dims.lY;
		m_mouseStateRelease.lZ = (m_mouseState.lX ^ dims.lZ) & ~dims.lZ;

		m_mouseState = dims;
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
	return (m_mouseState.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(トリガー)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseTrigger(int nButton)
{
	return (m_mouseStateTrigger.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リリース)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRelease(int nButton)
{
	return (m_mouseStateRelease.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// キーボードデータ取得(リピート)
//--------------------------------------------------------------------------------
//BOOL CMouseDX::GetMouseRepeat(int nButton)
//{
//	return (m_aMouseStateRepeat[nButton] & 0x80) ? TRUE : FALSE;
//}

//--------------------------------------------------------------------------------
// マウスデータ削除(トリガー)
//--------------------------------------------------------------------------------
void CMouseDX::FlushMouseTrigger(int nButton)
{
	m_mouseStateTrigger.rgbButtons[nButton] = 0;
}

//--------------------------------------------------------------------------------
// マウスX軸取得
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisX(void)
{
	return m_mouseState.lX;
}

//--------------------------------------------------------------------------------
// マウスY軸取得
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisY(void)
{
	return m_mouseState.lY;
}

//--------------------------------------------------------------------------------
// マウスZ軸取得(Wheel)
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisZ(void)
{
	return m_mouseState.lZ;
}

//--------------------------------------------------------------------------------
//
// CJoystickDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// コンストラスタ
//--------------------------------------------------------------------------------
CJoystickDX::CJoystickDX() : CInputDX()
	, m_hWnd(NULL)
	, m_bAttached(false)
{
	// 各ワークのクリア
	ZeroMemory(&m_joyState, sizeof DIJOYSTATE2);
	ZeroMemory(&m_joyStateTrigger, sizeof DIJOYSTATE2);
	ZeroMemory(&m_joyStateRelease, sizeof DIJOYSTATE2);
}

//--------------------------------------------------------------------------------
// ジョイスティックの初期化処理
//--------------------------------------------------------------------------------
HRESULT CJoystickDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	m_hWnd = hWnd;

	// 入力処理の初期化
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Inputデバイスが生成に失敗！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// Joystickデバイスを作成
	hr = AttachJoystick();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// マウスの更新処理
//--------------------------------------------------------------------------------
void CJoystickDX::Update(void)
{
	HRESULT hr;
	DIJOYSTATE2 diJoy;

	if (!m_pDIDevice) { return; }
	//{ 
	//	hr = AttachJoystick();
	//	if (FAILED(hr) || !m_pDIDevice) { return; }
	//}

	// 状態を取得する前にデータをポーリング
	hr = m_pDIDevice->Poll();
	if (FAILED(hr))
	{
		m_bAttached = false;
		m_pDIDevice->Acquire();
	}

	hr = m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &diJoy);

	if (SUCCEEDED(hr))
	{
		m_bAttached = true;

		//Button更新
		for (int nCntButton = 0; nCntButton < sizeof(diJoy.rgbButtons); nCntButton++)
		{
			m_joyStateTrigger.rgbButtons[nCntButton] = (m_joyState.rgbButtons[nCntButton] ^ diJoy.rgbButtons[nCntButton]) & diJoy.rgbButtons[nCntButton];
			m_joyStateRelease.rgbButtons[nCntButton] = (m_joyState.rgbButtons[nCntButton] ^ diJoy.rgbButtons[nCntButton]) & ~diJoy.rgbButtons[nCntButton];
		}

		//LStick更新
		m_joyStateTrigger.lX = (m_joyState.lX ^ diJoy.lX) & diJoy.lX;
		m_joyStateRelease.lX = (m_joyState.lX ^ diJoy.lX) & ~diJoy.lX;
											  			  
		m_joyStateTrigger.lY = (m_joyState.lY ^ diJoy.lY) & diJoy.lY;
		m_joyStateRelease.lY = (m_joyState.lY ^ diJoy.lY) & ~diJoy.lY;
											  			  
		m_joyStateTrigger.lZ = (m_joyState.lZ ^ diJoy.lZ) & diJoy.lZ;
		m_joyStateRelease.lZ = (m_joyState.lZ ^ diJoy.lZ) & ~diJoy.lZ;

		//RStick更新
		m_joyStateTrigger.lRx = (m_joyState.lRx ^ diJoy.lRx) & diJoy.lRx;
		m_joyStateRelease.lRx = (m_joyState.lRx ^ diJoy.lRx) & ~diJoy.lRx;
															 
		m_joyStateTrigger.lRy = (m_joyState.lRy ^ diJoy.lRy) & diJoy.lRy;
		m_joyStateRelease.lRy = (m_joyState.lRy ^ diJoy.lRy) & ~diJoy.lRy;
															 
		m_joyStateTrigger.lRz = (m_joyState.lRz ^ diJoy.lRz) & diJoy.lRz;
		m_joyStateRelease.lRz = (m_joyState.lRz ^ diJoy.lRz) & ~diJoy.lRz;

		//2つの追加軸
		for (int nCntSlider = 0; nCntSlider < 2; nCntSlider++)
		{
			//キートリガー、リリース情報の取得
			m_joyStateTrigger.rglSlider[nCntSlider] = (m_joyState.rglSlider[nCntSlider] ^ diJoy.rglSlider[nCntSlider]) & diJoy.rglSlider[nCntSlider];
			m_joyStateRelease.rglSlider[nCntSlider] = (m_joyState.rglSlider[nCntSlider] ^ diJoy.rglSlider[nCntSlider]) & ~diJoy.rglSlider[nCntSlider];
		}

		//Pov
		for (int nCntPov = 0; nCntPov < sizeof(diJoy.rgdwPOV); nCntPov++)
		{
			m_joyStateTrigger.rgdwPOV[nCntPov] = (m_joyState.rgdwPOV[nCntPov] != diJoy.rgdwPOV[nCntPov]) * diJoy.rgdwPOV[nCntPov];
			m_joyStateRelease.rgdwPOV[nCntPov] = (m_joyState.rgdwPOV[nCntPov] != diJoy.rgdwPOV[nCntPov]) * m_joyState.rgdwPOV[nCntPov];
		}

		m_joyState = diJoy;
	}
	else if (hr == DIERR_INPUTLOST)
	{
		m_bAttached = false;
		m_pDIDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
//  列挙されたデバイスごとに呼び出されるコールバック関数
//--------------------------------------------------------------------------------
BOOL CALLBACK CJoystickDX::EnumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext)
{
	DIDEVCAPS diDevCaps;				// Joystickの能力情報
	HRESULT hr;
	LPDIRECTINPUTDEVICE8* ppDevice = (LPDIRECTINPUTDEVICE8*)pContext;

	//列挙されたJoystickへのインターフェイスを取得する
	hr = m_pDInput->CreateDevice(pDidInstance->guidInstance, ppDevice, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;//列挙を続ける
	}

	//Joystickの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = (*ppDevice)->GetCapabilities(&diDevCaps);
	if (FAILED(hr))
	{
		//Joystick能力の取得に失敗
		(*ppDevice)->Release();
		(*ppDevice) = NULL;
		return DIENUM_CONTINUE;//列挙を続ける
	}

	//このデバイスを使う
	return DIENUM_STOP;//列挙を終了する場合
					   //return DIENUM_CONTINUE;//他のJoystickも探す場合
}

//--------------------------------------------------------------------------------
//  軸ごとに呼び出されるコールバック関数
//--------------------------------------------------------------------------------
BOOL CALLBACK CJoystickDX::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	DIPROPRANGE diprg;
	LPDIRECTINPUTDEVICE8* ppDevice = (LPDIRECTINPUTDEVICE8*)pvRef;

	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -sc_nStickAxisMax;
	diprg.lMax = +sc_nStickAxisMax;

	if (FAILED((*ppDevice)->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//--------------------------------------------------------------------------------
// ジョイスティックボタンデータ取得(プレス)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonPress(const XBOX_BUTTON& button)
{
	return (m_joyState.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティックボタンデータ取得(トリガー)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonTrigger(const XBOX_BUTTON& button)
{
	return (m_joyStateTrigger.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティックボタンデータ取得(リリース)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonRelease(const XBOX_BUTTON& button)
{
	return (m_joyStateRelease.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティック十字キーデータ取得(プレス)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyPress(const XBOX_CROSS_KEY& key)
{
	return (m_joyState.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティック十字キーデータ取得(トリガー)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyTrigger(const XBOX_CROSS_KEY& key)
{
	return (m_joyStateTrigger.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティック十字キーデータ取得(リリース)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyRelease(const XBOX_CROSS_KEY& key)
{
	return (m_joyStateRelease.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// ジョイスティックLStickXデータ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLStickAxisX(void)
{
	return m_joyState.lX;
}

//--------------------------------------------------------------------------------
// ジョイスティックLStickYデータ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLStickAxisY(void)
{
	return m_joyState.lY;
}

//--------------------------------------------------------------------------------
// ジョイスティックLT&RTデータ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLTandRT(void)
{
	return m_joyState.lZ;
}

//--------------------------------------------------------------------------------
// ジョイスティックRStickXデータ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRStickAxisX(void)
{
	return m_joyState.lRx;
}

//--------------------------------------------------------------------------------
// ジョイスティックRStickYデータ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRStickAxisY(void)
{
	return m_joyState.lRy;
}

//--------------------------------------------------------------------------------
// ジョイスティックRudder(方向舵)データ取得
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRudder(void)
{
	return m_joyState.lRz;
}

//--------------------------------------------------------------------------------
// ジョイスティックと連結する、成功したらデバイスを作る
//--------------------------------------------------------------------------------
HRESULT CJoystickDX::AttachJoystick(void)
{
	HRESULT hr;

	// Joystickデバイスを列挙して作成
	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, &m_pDIDevice, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		m_bAttached = false;
		//MessageBox(hWnd, "Joystickデバイスを列挙して作成に失敗！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystickのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystickの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 軸の値の範囲の設定
	hr = m_pDIDevice->EnumObjects(EnumAxesCallback, &m_pDIDevice, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystickの軸の値の範囲を設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	m_bAttached = true;

	return hr;
}
//--------------------------------------------------------------------------------
//	入力情報管理処理
//　inputManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "inputManager.h"
#include "inputDX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CInputManager::CInputManager()
	: m_pKeyboard(nullptr)
	, m_pMouse(nullptr)
	, m_pJoystick(nullptr)
	, m_fMoveHorizontal(0.0f)
	, m_fMoveVertical(0.0f)
	, m_fRotHorizontal(0.0f)
	, m_fRotVertical(0.0f)
	, m_fZoom(0.0f)
	, m_lKeysPress(0)
	, m_lKeysTrigger(0)
	, m_lKeysRelease(0)
	, m_bEditorMode(false)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CInputManager::~CInputManager()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CInputManager::Init(HINSTANCE hInst, HWND hWnd)
{
	m_pKeyboard = new CKeyboardDX;
	m_pKeyboard->Init(hInst, hWnd);
	m_pMouse = new CMouseDX;
	m_pMouse->Init(hInst, hWnd);
	m_pJoystick = new CJoystickDX;
	m_pJoystick->Init(hInst, hWnd);
	return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CInputManager::Update(void)
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pJoystick->Update();
	updateInputInfo();
}

//--------------------------------------------------------------------------------
//  使用権取得
//--------------------------------------------------------------------------------
void CInputManager::Acquire(void)
{
	m_pKeyboard->Acquire();
	m_pMouse->Acquire();
	m_pJoystick->Acquire();
}

//--------------------------------------------------------------------------------
//  使用権解放
//--------------------------------------------------------------------------------
void CInputManager::Unacquire(void)
{
	m_pKeyboard->Unacquire();
	m_pMouse->Unacquire();
	m_pJoystick->Unacquire();
}

//--------------------------------------------------------------------------------
//  キーのプレス
//--------------------------------------------------------------------------------
bool CInputManager::GetKeyPress(const KEY& key)
{
	return m_lKeysPress & (1 << (int)key);
}

//--------------------------------------------------------------------------------
//  キーのトリガー
//--------------------------------------------------------------------------------
bool CInputManager::GetKeyTrigger(const KEY& key)
{
	return m_lKeysTrigger & (1 << (int)key);
}

//--------------------------------------------------------------------------------
//  キーのリリース
//--------------------------------------------------------------------------------
bool CInputManager::GetKeyRelease(const KEY& key)
{
	return m_lKeysRelease & (1 << (int)key);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  入力情報更新処理
//--------------------------------------------------------------------------------
void CInputManager::updateInputInfo(void)
{
	//Move
	float fKAxisX = -(float)m_pKeyboard->GetKeyPress(DIK_A) + (float)m_pKeyboard->GetKeyPress(DIK_D);
	float fKAxisY = (float)m_pKeyboard->GetKeyPress(DIK_W) - (float)m_pKeyboard->GetKeyPress(DIK_S);
	float fJLAxisX = (float)m_pJoystick->GetLStickAxisX() / CJoystickDX::sc_nStickAxisMax;
	float fJLAxisY = -(float)m_pJoystick->GetLStickAxisY() / CJoystickDX::sc_nStickAxisMax;
	fJLAxisX = fabsf(fJLAxisX) > 0.1f ? fJLAxisX : 0.0f;
	fJLAxisY = fabsf(fJLAxisY) > 0.1f ? fJLAxisY : 0.0f;
	m_fMoveHorizontal = fabsf(fKAxisX) > fabsf(fJLAxisX) ? fKAxisX : fJLAxisX;
	m_fMoveVertical = fabsf(fKAxisY) > fabsf(fJLAxisY) ? fKAxisY : fJLAxisY;

	//Rot
	float fMAxisX = 0.0f;
	float fMAxisY = 0.0f;
	if (m_bEditorMode)
	{
		fMAxisX = (float)m_pMouse->GetMouseAxisX() * 0.2f;
		fMAxisY = (float)m_pMouse->GetMouseAxisY() * 0.2f;
	}
	else
	{
		fMAxisX = -(float)m_pKeyboard->GetKeyPress(DIK_Q) + (float)m_pKeyboard->GetKeyPress(DIK_E);
		fMAxisY = -(float)m_pKeyboard->GetKeyPress(DIK_R) + (float)m_pKeyboard->GetKeyPress(DIK_T);
	}
	float fJRAxisX = (float)m_pJoystick->GetRStickAxisX() / CJoystickDX::sc_nStickAxisMax;
	float fJRAxisY = (float)m_pJoystick->GetRStickAxisY() / CJoystickDX::sc_nStickAxisMax;
	m_fRotHorizontal = fabsf(fMAxisX) > fabsf(fJRAxisX) ? fMAxisX : fJRAxisX;
	m_fRotVertical = fabsf(fMAxisY) > fabsf(fJRAxisY) ? fMAxisY : fJRAxisY;

	//Zoom
	float fMAxisZ = -(float)m_pMouse->GetMouseAxisZ() / MOUSE_AXIS_MAX;
	float fJAxisZ = (float)m_pJoystick->GetLTandRT() / CJoystickDX::sc_nStickAxisMax;
	m_fZoom = fabsf(fMAxisZ) > fabsf(fJAxisZ) ? fMAxisZ : fJAxisZ;

	//Key
	//Submit
	bool bSubmitPress = m_pKeyboard->GetKeyPress(DIK_SPACE) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_A);
	bool bSubmitTrigger = m_pKeyboard->GetKeyTrigger(DIK_SPACE) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_A);
	bool bSubmitRelease = m_pKeyboard->GetKeyRelease(DIK_SPACE) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_A);

	//Cancel
	bool bCancelPress = m_pKeyboard->GetKeyPress(DIK_J) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_B);
	bool bCancelTrigger = m_pKeyboard->GetKeyTrigger(DIK_J) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_B);
	bool bCancelRelease = m_pKeyboard->GetKeyRelease(DIK_J) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_B);

	//Start
	bool bStartPress = m_pKeyboard->GetKeyPress(DIK_RETURN) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_MENU);
	bool bStartTrigger = m_pKeyboard->GetKeyTrigger(DIK_RETURN) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_MENU);
	bool bStartRelease = m_pKeyboard->GetKeyRelease(DIK_RETURN) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_MENU);

	//RotX
	bool bRotXMinusPress = m_pKeyboard->GetKeyPress(DIK_T);
	bool bRotXMinusTrigger = m_pKeyboard->GetKeyTrigger(DIK_T);
	bool bRotXMinusRelease = m_pKeyboard->GetKeyRelease(DIK_T);
	bool bRotXPlusPress = m_pKeyboard->GetKeyPress(DIK_Y);
	bool bRotXPlusTrigger = m_pKeyboard->GetKeyTrigger(DIK_Y);
	bool bRotXPlusRelease = m_pKeyboard->GetKeyRelease(DIK_Y);

	//RotY
	bool bRotYMinusPress = m_pKeyboard->GetKeyPress(DIK_G);
	bool bRotYMinusTrigger = m_pKeyboard->GetKeyTrigger(DIK_G);
	bool bRotYMinusRelease = m_pKeyboard->GetKeyRelease(DIK_G);
	bool bRotYPlusPress = m_pKeyboard->GetKeyPress(DIK_H);
	bool bRotYPlusTrigger = m_pKeyboard->GetKeyTrigger(DIK_H);
	bool bRotYPlusRelease = m_pKeyboard->GetKeyRelease(DIK_H);

	//RotZ
	bool bRotZMinusPress = m_pKeyboard->GetKeyPress(DIK_B);
	bool bRotZMinusTrigger = m_pKeyboard->GetKeyTrigger(DIK_B);
	bool bRotZMinusRelease = m_pKeyboard->GetKeyRelease(DIK_B);
	bool bRotZPlusPress = m_pKeyboard->GetKeyPress(DIK_N);
	bool bRotZPlusTrigger = m_pKeyboard->GetKeyTrigger(DIK_N);
	bool bRotZPlusRelease = m_pKeyboard->GetKeyRelease(DIK_N);

	//Up
	bool bUpPress = m_pKeyboard->GetKeyPress(DIK_UP);
	bool bUpTrigger = m_pKeyboard->GetKeyTrigger(DIK_UP);
	bool bUpRelease = m_pKeyboard->GetKeyRelease(DIK_UP);

	//Down
	bool bDownPress = m_pKeyboard->GetKeyPress(DIK_DOWN);
	bool bDownTrigger = m_pKeyboard->GetKeyTrigger(DIK_DOWN);
	bool bDownRelease = m_pKeyboard->GetKeyRelease(DIK_DOWN);

	//Left
	bool bLeftPress = m_pKeyboard->GetKeyPress(DIK_LEFT);
	bool bLeftTrigger = m_pKeyboard->GetKeyTrigger(DIK_LEFT);
	bool bLeftRelease = m_pKeyboard->GetKeyRelease(DIK_LEFT);

	//Right
	bool bRightPress = m_pKeyboard->GetKeyPress(DIK_RIGHT);
	bool bRightTrigger = m_pKeyboard->GetKeyTrigger(DIK_RIGHT);
	bool bRightRelease = m_pKeyboard->GetKeyRelease(DIK_RIGHT);

	//LeftRacket
	bool bLRacketPress = m_pKeyboard->GetKeyPress(DIK_LBRACKET);
	bool bLRacketTrigger = m_pKeyboard->GetKeyTrigger(DIK_LBRACKET);
	bool bLRacketRelease = m_pKeyboard->GetKeyRelease(DIK_LBRACKET);

	//RightRacket
	bool bRRacketPress = m_pKeyboard->GetKeyPress(DIK_RBRACKET);
	bool bRRacketTrigger = m_pKeyboard->GetKeyTrigger(DIK_RBRACKET);
	bool bRRacketRelease = m_pKeyboard->GetKeyRelease(DIK_RBRACKET);

	//Reset
	bool bResetPress = m_pKeyboard->GetKeyPress(DIK_R);
	bool bResetTrigger = m_pKeyboard->GetKeyTrigger(DIK_R);
	bool bResetRelease = m_pKeyboard->GetKeyRelease(DIK_R);

	m_lKeysPress =
		(LONG)bSubmitPress << K_SUBMIT
		| (bCancelPress << K_CANCEL)
		| (bStartPress << K_START)
		| (bRotXMinusPress << K_ROTXM)
		| (bRotXPlusPress << K_ROTXP)
		| (bRotYMinusPress << K_ROTYM)
		| (bRotYPlusPress << K_ROTYP)
		| (bRotZMinusPress << K_ROTZM)
		| (bRotZPlusPress << K_ROTZP)
		| (bUpPress << K_UP)
		| (bDownPress << K_DOWN)
		| (bLeftPress << K_LEFT)
		| (bRightPress << K_RIGHT)
		| (bLRacketPress << K_LRACKET)
		| (bRRacketPress << K_RRACKET)
		| (bResetPress << K_RESET);

	m_lKeysTrigger =
		(LONG)bSubmitTrigger << K_SUBMIT
		| (bCancelTrigger << K_CANCEL)
		| (bStartTrigger << K_START)
		| (bRotXMinusTrigger << K_ROTXM)
		| (bRotXPlusTrigger << K_ROTXP)
		| (bRotYMinusTrigger << K_ROTYM)
		| (bRotYPlusTrigger << K_ROTYP)
		| (bRotZMinusTrigger << K_ROTZM)
		| (bRotZPlusTrigger << K_ROTZP)
		| (bUpTrigger << K_UP)
		| (bDownTrigger << K_DOWN)
		| (bLeftTrigger << K_LEFT)
		| (bRightTrigger << K_RIGHT)
		| (bLRacketTrigger << K_LRACKET)
		| (bRRacketTrigger << K_RRACKET)
		| (bResetTrigger << K_RESET);

	m_lKeysRelease =
		(LONG)bSubmitRelease << K_SUBMIT
		| (bCancelRelease << K_CANCEL)
		| (bStartRelease << K_START)
		| (bRotXMinusRelease << K_ROTXM)
		| (bRotXPlusRelease << K_ROTXP)
		| (bRotYMinusRelease << K_ROTYM)
		| (bRotYPlusRelease << K_ROTYP)
		| (bRotZMinusRelease << K_ROTZM)
		| (bRotZPlusRelease << K_ROTZP)
		| (bUpRelease << K_UP)
		| (bDownRelease << K_DOWN)
		| (bLeftRelease << K_LEFT)
		| (bRightRelease << K_RIGHT)
		| (bLRacketRelease << K_LRACKET)
		| (bRRacketRelease << K_RRACKET)
		| (bResetRelease << K_RESET);

#ifdef _DEBUG
	//char str[512];
	//sprintf(str, "MX : %f\tMY : %f\tMZ : %f\n", fMAxisX, fMAxisY, fMAxisZ);
	//sprintf(str, "JRX : %f\tJRY : %f\tJRZ : %f\n", fJRAxisX, fJRAxisY, fJAxisZ);
	/*sprintf(str, "JLX : %f\tJLY : %f\t\n", fJLAxisX, fJLAxisY);
	OutputDebugString(str);*/
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CInputManager::uninit(void)
{
	//キーボードの破棄
	if (m_pKeyboard)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//マウスの破棄
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	//ジョイスティックの破棄
	if (m_pJoystick)
	{
		m_pJoystick->Uninit();
		delete m_pJoystick;
		m_pJoystick = nullptr;
	}
}
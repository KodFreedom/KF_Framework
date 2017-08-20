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
//  終了処理
//--------------------------------------------------------------------------------
void CInputManager::Uninit(void)
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

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CInputManager::Update(void)
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pJoystick->Update();
	UpdateInputInfo();
}

//--------------------------------------------------------------------------------
//  入力情報更新処理
//--------------------------------------------------------------------------------
void CInputManager::UpdateInputInfo(void)
{
	//Move
	float fKAxisX = -(float)m_pKeyboard->GetKeyPress(DIK_A) + (float)m_pKeyboard->GetKeyPress(DIK_D);
	float fKAxisY = (float)m_pKeyboard->GetKeyPress(DIK_W) - (float)m_pKeyboard->GetKeyPress(DIK_S);
	float fJLAxisX = (float)m_pJoystick->GetLStickAxisX() / CJoystickDX::sc_nStickAxisMax;
	float fJLAxisY = (float)m_pJoystick->GetLStickAxisY() / CJoystickDX::sc_nStickAxisMax;
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
	//Jump
	bool bJumpPress = m_pKeyboard->GetKeyPress(DIK_SPACE) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_A);
	bool bJumpTrigger = m_pKeyboard->GetKeyTrigger(DIK_SPACE) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_A);
	bool bJumpRelease = m_pKeyboard->GetKeyRelease(DIK_SPACE) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_A);
	
	//Attack
	bool bAttackPress = m_pKeyboard->GetKeyPress(DIK_J) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_B);
	bool bAttackTrigger = m_pKeyboard->GetKeyTrigger(DIK_J) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_B);
	bool bAttackRelease = m_pKeyboard->GetKeyRelease(DIK_J) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_B);
	
	//Submit
	bool bSubmitPress = m_pKeyboard->GetKeyPress(DIK_RETURN) | m_pJoystick->GetButtonPress(CJoystickDX::XBOX_BUTTON::B_MENU);
	bool bSubmitTrigger = m_pKeyboard->GetKeyTrigger(DIK_RETURN) | m_pJoystick->GetButtonTrigger(CJoystickDX::XBOX_BUTTON::B_MENU);
	bool bSubmitRelease = m_pKeyboard->GetKeyRelease(DIK_RETURN) | m_pJoystick->GetButtonRelease(CJoystickDX::XBOX_BUTTON::B_MENU);

	//Extend
	bool bExtendPress = m_pKeyboard->GetKeyPress(DIK_F);
	bool bExtendTrigger = m_pKeyboard->GetKeyTrigger(DIK_F);
	bool bExtendRelease = m_pKeyboard->GetKeyRelease(DIK_F);

	//Shrink
	bool bShrinkPress = m_pKeyboard->GetKeyPress(DIK_G);
	bool bShrinkTrigger = m_pKeyboard->GetKeyTrigger(DIK_G);
	bool bShrinkRelease = m_pKeyboard->GetKeyRelease(DIK_G);

	//Raise
	bool bRaisePress = m_pKeyboard->GetKeyPress(DIK_V);
	bool bRaiseTrigger = m_pKeyboard->GetKeyTrigger(DIK_V);
	bool bRaiseRelease = m_pKeyboard->GetKeyRelease(DIK_V);

	//Reduce
	bool bReducePress = m_pKeyboard->GetKeyPress(DIK_B);
	bool bReduceTrigger = m_pKeyboard->GetKeyTrigger(DIK_B);
	bool bReduceRelease = m_pKeyboard->GetKeyRelease(DIK_B);

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

	m_lKeysPress = (LONG)bJumpPress
		| (bAttackPress << 1)
		| (bSubmitPress << 2)
		| (bExtendPress << 3)
		| (bShrinkPress << 4)
		| (bRaisePress << 5)
		| (bReducePress << 6)
		| (bUpPress << 7)
		| (bDownPress << 8)
		| (bLeftPress << 9)
		| (bRightPress << 10);

	m_lKeysTrigger = (LONG)bJumpTrigger
		| (bAttackTrigger << 1)
		| (bSubmitTrigger << 2)
		| (bExtendTrigger << 3)
		| (bShrinkTrigger << 4)
		| (bRaiseTrigger << 5)
		| (bReduceTrigger << 6)
		| (bUpTrigger << 7)
		| (bDownTrigger << 8)
		| (bLeftTrigger << 9)
		| (bRightTrigger << 10);

	m_lKeysRelease = (LONG)bJumpRelease
		| (bAttackRelease << 1)
		| (bSubmitRelease << 2)
		| (bExtendRelease << 3)
		| (bShrinkRelease << 4)
		| (bRaiseRelease << 5)
		| (bReduceRelease << 6)
		| (bUpRelease << 7)
		| (bDownRelease << 8)
		| (bLeftRelease << 9)
		| (bRightRelease << 10);
	
#ifdef _DEBUG
	//char str[512];
	//sprintf(str, "MX : %f\tMY : %f\tMZ : %f\n", fMAxisX, fMAxisY, fMAxisZ);
	//sprintf(str, "JRX : %f\tJRY : %f\tJRZ : %f\n", fJRAxisX, fJRAxisY, fJAxisZ);
	/*sprintf(str, "JLX : %f\tJLY : %f\t\n", fJLAxisX, fJLAxisY);
	OutputDebugString(str);*/
#endif
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

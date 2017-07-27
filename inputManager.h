//--------------------------------------------------------------------------------
//	入力情報管理処理
//　inputManager.h
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CKeyboardDX;
class CMouseDX;
class CJoystickDX;

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class CInputManager
{
public:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	enum KEY
	{
		K_JUMP = 0x01,
		K_ATTACK = 0x02,
		K_SUBMIT = 0x04,
		K_CANCEL = 0x02,
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CInputManager();
	~CInputManager();

	bool			Init(HINSTANCE hInst, HWND hWnd);
	void			Uninit(void);
	void			Update(void);

	//
	void			Acquire(void);
	void			Unacquire(void);

	//Get関数
	CKeyboardDX*	GetKeyboard(void) { return m_pKeyboard; }
	CMouseDX*		GetMouse(void) { return m_pMouse; }
	CJoystickDX*	GetJoystickDX(void) { return m_pJoystick; }
	float			GetMoveHorizontal(void) { return m_fMoveHorizontal; }
	float			GetMoveVertical(void) { return m_fMoveVertical; }
	float			GetRotHorizontal(void) { return m_fRotHorizontal; }
	float			GetRotVertical(void) { return m_fRotVertical; }
	float			GetZoom(void) { return m_fZoom; }
	bool			GetKeyPress(const KEY& key);
	bool			GetKeyTrigger(const KEY& key);
	bool			GetKeyRelease(const KEY& key);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void			UpdateInputInfo(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	//入力デバイス
	CKeyboardDX*	m_pKeyboard;
	CMouseDX*		m_pMouse;
	CJoystickDX*	m_pJoystick;

	//入力情報
	float			m_fMoveHorizontal;	//移動用横情報
	float			m_fMoveVertical;	//移動用縦情報
	float			m_fRotHorizontal;	//回転用横情報
	float			m_fRotVertical;		//回転用縦情報
	float			m_fZoom;			//ズーム用情報
	LONG			m_lKeysPress;		//キーフラッグ情報
	LONG			m_lKeysTrigger;		//キートリガーフラッグ情報
	LONG			m_lKeysRelease;		//キーリリースフラッグ情報

};
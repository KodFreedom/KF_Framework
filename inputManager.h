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
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum KEY
	{
		K_SUBMIT	= 0,
		K_CANCEL	= 1,
		K_START		= 2,
		K_ROTXM		= 3,
		K_ROTXP		= 4,
		K_ROTYM		= 5,
		K_ROTYP		= 6,
		K_ROTZM		= 7,
		K_ROTZP		= 8,
		K_UP		= 9,
		K_DOWN		= 10,
		K_LEFT		= 11,
		K_RIGHT		= 12,
		K_LRACKET	= 13,
		K_RRACKET	= 14,
		K_RESET		= 15,

		//重ねるキー
		K_JUMP		= 0,
		K_ATTACK	= 1,
		K_SAVE		= 2,
		K_EXTEND	= 3,
		K_SHRINK	= 4,
		K_RAISE		= 5,
		K_REDUCE	= 6,
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CInputManager();
	~CInputManager();

	bool			Init(HINSTANCE hInst, HWND hWnd);
	void			Release(void)
	{
		uninit();
		delete this;
	}
	void			Update(void);

	//使用権の確保、解放
	void			Acquire(void);
	void			Unacquire(void);

	//Set関数
	void			SetEditorMode(const bool& bUse) { m_bEditorMode = bUse; }

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
	void			updateInputInfo(void);
	void			uninit(void);

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
	bool			m_bEditorMode;
};
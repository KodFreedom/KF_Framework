//--------------------------------------------------------------------------------
//	DirectX入力処理
//　inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _INPUT_DX_H_
#define _INPUT_DX_H_

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define MOUSE_AXIS_MAX (120.0f)		//マウスの軸値の最大値

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// 入力クラス
//--------------------------------------------------------------------------------
class CInputDX
{
public:
	CInputDX();
	virtual ~CInputDX() {}

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void	Uninit(void);
	virtual void	Update(void) = 0;

	void Acquire(void) { if (m_pDIDevice) { m_pDIDevice->Acquire(); } }
	void Unacquire(void) { if (m_pDIDevice) { m_pDIDevice->Unacquire(); } }

protected:
	static LPDIRECTINPUT8	m_pDInput;	// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8	m_pDIDevice;	// Deviceオブジェクト(入力に必要)
};

//--------------------------------------------------------------------------------
// キーボード入力クラス
//--------------------------------------------------------------------------------
class CKeyboardDX : public CInputDX
{
public:
	CKeyboardDX();
	~CKeyboardDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);
	
private:
	static const int m_nNumKeyMax = 256;		// キー最大数
	BYTE	m_aKeyState[m_nNumKeyMax];			// キーボードの状態を受け取るワーク
	BYTE	m_aKeyStateTrigger[m_nNumKeyMax];	// トリガーワーク
	BYTE	m_aKeyStateRelease[m_nNumKeyMax];	// リリースワーク
	BYTE	m_aKeyStateRepeat[m_nNumKeyMax];	// リピートワーク
	int		m_aKeyStateRepeatCnt[m_nNumKeyMax];	// リピートカウンタ
};

//--------------------------------------------------------------------------------
// マウス入力クラス
//--------------------------------------------------------------------------------
class CMouseDX : public CInputDX
{
public:
	enum MOUSE_BUTTON 
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_MAX
	};

	CMouseDX();
	~CMouseDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Update(void);

	BOOL GetMousePress(int nButton);
	BOOL GetMouseTrigger(int nButton);
	BOOL GetMouseRelease(int nButton);
	BOOL GetMouseRepeat(int nButton);
	void FlushMouseTrigger(int nButton);
	LONG GetMouseAxisX(void);
	LONG GetMouseAxisY(void);
	LONG GetMouseAxisZ(void);

private:
	static const int m_nNumMBMax = 8;				// マウスボタン最大数
	BYTE	m_aMouseState[m_nNumMBMax];				// キーボードの状態を受け取るワーク
	BYTE	m_aMouseStateTrigger[m_nNumMBMax];		// トリガーワーク
	BYTE	m_aMouseStateRelease[m_nNumMBMax];		// リリースワーク
	BYTE	m_aMouseStateRepeat[m_nNumMBMax];		// リピートワーク
	int		m_aMouseStateRepeatCnt[m_nNumMBMax];	// リピートカウンタ
	LONG	m_lMouseX;
	LONG	m_lMouseY;
	LONG	m_lMouseZ;
};

#endif
//--------------------------------------------------------------------------------
//	DirectX���͏���
//�@inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define MOUSE_AXIS_MAX (120.0f)		//�}�E�X�̎��l�̍ő�l

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// ���̓N���X
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
	static const int sc_nCntForRepeat = 20; // ���s�[�g�҂�����
	static LPDIRECTINPUT8	m_pDInput;		// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	m_pDIDevice;	// Device�I�u�W�F�N�g(���͂ɕK�v)
};

//--------------------------------------------------------------------------------
// �L�[�{�[�h���̓N���X
//--------------------------------------------------------------------------------
class CKeyboardDX : public CInputDX
{
public:
	CKeyboardDX();
	~CKeyboardDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void Update(void) override;

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);
	
private:
	static const int sc_nNumKeyMax = 256;			// �L�[�ő吔
	BYTE	m_aKeyState[sc_nNumKeyMax];				// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE	m_aKeyStateTrigger[sc_nNumKeyMax];		// �g���K�[���[�N
	BYTE	m_aKeyStateRelease[sc_nNumKeyMax];		// �����[�X���[�N
	BYTE	m_aKeyStateRepeat[sc_nNumKeyMax];		// ���s�[�g���[�N
	int		m_aKeyStateRepeatCnt[sc_nNumKeyMax];	// ���s�[�g�J�E���^
};

//--------------------------------------------------------------------------------
// �}�E�X���̓N���X
//--------------------------------------------------------------------------------
class CMouseDX : public CInputDX
{
public:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum MOUSE_BUTTON 
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_MAX
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CMouseDX();
	~CMouseDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void Update(void) override;

	BOOL GetMousePress(int nButton);
	BOOL GetMouseTrigger(int nButton);
	BOOL GetMouseRelease(int nButton);
	//BOOL GetMouseRepeat(int nButton);
	void FlushMouseTrigger(int nButton);
	LONG GetMouseAxisX(void);
	LONG GetMouseAxisY(void);
	LONG GetMouseAxisZ(void);

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	DIMOUSESTATE2	m_mouseState;
	DIMOUSESTATE2	m_mouseStateTrigger;
	DIMOUSESTATE2	m_mouseStateRelease;
};

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N���̓N���X
//--------------------------------------------------------------------------------
class CJoystickDX : public CInputDX
{
public:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum XBOX_BUTTON
	{
		B_A = 0,
		B_B = 1,
		B_X = 2,
		B_Y = 3,
		B_LB = 4,
		B_RB = 5,
		B_BACK = 6,
		B_MENU = 7,
		B_LS = 8,
		B_RS = 9
	};

	enum XBOX_CROSS_KEY
	{
		K_UP = 0,
		K_DOWN = 18000,
		K_LEFT = 27000,
		K_RIGHT = 9000
	};

	enum XBOX_STICK
	{
		S_UP = -1000,
		S_DOWN = 1000,
		S_LEFT = -1000,
		S_RIGHT = 1000
	};

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int sc_nStickAxisMax = 1000;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CJoystickDX();
	~CJoystickDX() {}
	
	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void Update(void) override;

	BOOL GetButtonPress(const XBOX_BUTTON& button);
	BOOL GetButtonTrigger(const XBOX_BUTTON& button);
	BOOL GetButtonRelease(const XBOX_BUTTON& button);

	BOOL GetCrossKeyPress(const XBOX_CROSS_KEY& key);
	BOOL GetCrossKeyTrigger(const XBOX_CROSS_KEY& key);
	BOOL GetCrossKeyRelease(const XBOX_CROSS_KEY& key);

	LONG GetLStickAxisX(void);
	LONG GetLStickAxisY(void);
	LONG GetLTandRT(void);
	LONG GetRStickAxisX(void);
	LONG GetRStickAxisY(void);
	LONG GetRudder(void);

	bool GetAttached(void) { return m_bAttached; }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	HRESULT	AttachJoystick(void);
	static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext);
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	DIJOYSTATE2 m_joyState;
	DIJOYSTATE2 m_joyStateTrigger;
	DIJOYSTATE2 m_joyStateRelease;
	HWND		m_hWnd;
	bool		m_bAttached;
};
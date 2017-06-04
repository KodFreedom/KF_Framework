//--------------------------------------------------------------------------------
//	DirectX���͏���
//�@inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _INPUT_DX_H_
#define _INPUT_DX_H_

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
	static LPDIRECTINPUT8	m_pDInput;	// DirectInput�I�u�W�F�N�g
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

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);
	
private:
	static const int m_nNumKeyMax = 256;		// �L�[�ő吔
	BYTE	m_aKeyState[m_nNumKeyMax];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE	m_aKeyStateTrigger[m_nNumKeyMax];	// �g���K�[���[�N
	BYTE	m_aKeyStateRelease[m_nNumKeyMax];	// �����[�X���[�N
	BYTE	m_aKeyStateRepeat[m_nNumKeyMax];	// ���s�[�g���[�N
	int		m_aKeyStateRepeatCnt[m_nNumKeyMax];	// ���s�[�g�J�E���^
};

//--------------------------------------------------------------------------------
// �}�E�X���̓N���X
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
	static const int m_nNumMBMax = 8;				// �}�E�X�{�^���ő吔
	BYTE	m_aMouseState[m_nNumMBMax];				// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE	m_aMouseStateTrigger[m_nNumMBMax];		// �g���K�[���[�N
	BYTE	m_aMouseStateRelease[m_nNumMBMax];		// �����[�X���[�N
	BYTE	m_aMouseStateRepeat[m_nNumMBMax];		// ���s�[�g���[�N
	int		m_aMouseStateRepeatCnt[m_nNumMBMax];	// ���s�[�g�J�E���^
	LONG	m_lMouseX;
	LONG	m_lMouseY;
	LONG	m_lMouseZ;
};

#endif
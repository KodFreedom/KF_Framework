//--------------------------------------------------------------------------------
//	���͏��Ǘ�����
//�@inputManager.h
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CKeyboardDX;
class CMouseDX;
class CJoystickDX;

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
class CInputManager
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	enum KEY
	{
		K_JUMP = 0x01,
		K_ATTACK = 0x02,
		K_SUBMIT = 0x04,
		K_CANCEL = 0x02,
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CInputManager();
	~CInputManager();

	bool			Init(HINSTANCE hInst, HWND hWnd);
	void			Uninit(void);
	void			Update(void);

	//
	void			Acquire(void);
	void			Unacquire(void);

	//Get�֐�
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
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			UpdateInputInfo(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	//���̓f�o�C�X
	CKeyboardDX*	m_pKeyboard;
	CMouseDX*		m_pMouse;
	CJoystickDX*	m_pJoystick;

	//���͏��
	float			m_fMoveHorizontal;	//�ړ��p�����
	float			m_fMoveVertical;	//�ړ��p�c���
	float			m_fRotHorizontal;	//��]�p�����
	float			m_fRotVertical;		//��]�p�c���
	float			m_fZoom;			//�Y�[���p���
	LONG			m_lKeysPress;		//�L�[�t���b�O���
	LONG			m_lKeysTrigger;		//�L�[�g���K�[�t���b�O���
	LONG			m_lKeysRelease;		//�L�[�����[�X�t���b�O���

};
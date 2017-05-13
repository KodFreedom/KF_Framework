//--------------------------------------------------------------------------------
//	DirectX���͏���
//�@inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "inputDX.h"

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�҂�����

//--------------------------------------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------------------------------------
LPDIRECTINPUT8	CInputDX::m_pDInput = NULL;	// DirectInput�I�u�W�F�N�g

//--------------------------------------------------------------------------------
//
// CInputDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �R���X�g���X�^
//--------------------------------------------------------------------------------
CInputDX::CInputDX()
	: m_pDIDevice(NULL)
{

}

//--------------------------------------------------------------------------------
// ���͏����̏���������
//--------------------------------------------------------------------------------
HRESULT CInputDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̍쐬
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
// ���͏����̏I������
//--------------------------------------------------------------------------------
void CInputDX::Uninit(void)
{
	// �f�o�C�X�I�u�W�F�N�g�̔j��
	if (m_pDIDevice)
	{
		m_pDIDevice->Unacquire();
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
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
// �R���X�g���X�^
//--------------------------------------------------------------------------------
CKeyboardDX::CKeyboardDX()
{
	// �e���[�N�̃N���A
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�̏���������
//--------------------------------------------------------------------------------
HRESULT CKeyboardDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h���͏����̊J��
//--------------------------------------------------------------------------------
void CKeyboardDX::Uninit(void)
{
	// ���͏����̊J��
	CInputDX::Uninit();
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�̍X�V����
//--------------------------------------------------------------------------------
void CKeyboardDX::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[m_nNumKeyMax];

	if (!m_pDIDevice)
	{
		return;
	}

	// �f�o�C�X����f�[�^���擾
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
// �L�[�{�[�h�f�[�^�擾(�v���X)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�g���K�[)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�����[�X)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(���s�[�g)
//--------------------------------------------------------------------------------
BOOL CKeyboardDX::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�폜(�g���K�[)
//--------------------------------------------------------------------------------
void CKeyboardDX::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

//--------------------------------------------------------------------------------
// CMouseDX�R���X�g���X�^
//--------------------------------------------------------------------------------
CMouseDX::CMouseDX()
{
	// �e���[�N�̃N���A
	ZeroMemory(m_aMouseState, sizeof m_aMouseState);
	ZeroMemory(m_aMouseStateTrigger, sizeof m_aMouseStateTrigger);
	ZeroMemory(m_aMouseStateRelease, sizeof m_aMouseStateRelease);
	ZeroMemory(m_aMouseStateRepeat, sizeof m_aMouseStateRepeat);
	ZeroMemory(m_aMouseStateRepeatCnt, sizeof m_aMouseStateRepeatCnt);
}

//--------------------------------------------------------------------------------
// �}�E�X�̏���������
//--------------------------------------------------------------------------------
HRESULT CMouseDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	//// �f�o�C�X�̐ݒ�    
	//DIPROPDWORD diprop;
	//diprop.diph.dwSize = sizeof(diprop);
	//diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	//diprop.diph.dwObj = 0;
	//diprop.diph.dwHow = DIPH_DEVICE;
	//diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
	//m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

//--------------------------------------------------------------------------------
// �}�E�X�̍X�V����
//--------------------------------------------------------------------------------
void CMouseDX::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 dims;

	hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	
	if (SUCCEEDED(hr))
	{
		//Button�X�V
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

		//�}�E�X�ʒu�X�V
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
// �L�[�{�[�h�f�[�^�擾(�v���X)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMousePress(int nButton)
{
	return (m_aMouseState[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�g���K�[)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseTrigger(int nButton)
{
	return (m_aMouseStateTrigger[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�����[�X)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRelease(int nButton)
{
	return (m_aMouseStateRelease[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(���s�[�g)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRepeat(int nButton)
{
	return (m_aMouseStateRepeat[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �}�E�X�f�[�^�폜(�g���K�[)
//--------------------------------------------------------------------------------
void CMouseDX::FlushMouseTrigger(int nButton)
{
	m_aMouseStateTrigger[nButton] = 0;
}

//--------------------------------------------------------------------------------
// �}�E�XX���擾
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisX(void)
{
	return m_lMouseX;
}

//--------------------------------------------------------------------------------
// �}�E�XY���擾
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisY(void)
{
	return m_lMouseY;
}

//--------------------------------------------------------------------------------
// �}�E�XZ���擾(Wheel)
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisZ(void)
{
	return m_lMouseZ;
}
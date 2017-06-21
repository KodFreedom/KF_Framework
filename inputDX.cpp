//--------------------------------------------------------------------------------
//	DirectX���͏���
//�@inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "inputDX.h"

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
CKeyboardDX::CKeyboardDX() : CInputDX()
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
// �L�[�{�[�h�̍X�V����
//--------------------------------------------------------------------------------
void CKeyboardDX::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[sc_nNumKeyMax];

	if (!m_pDIDevice)
	{
		return;
	}

	// �f�o�C�X����f�[�^���擾
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
//
// CMouseDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �R���X�g���X�^
//--------------------------------------------------------------------------------
CMouseDX::CMouseDX() : CInputDX()
{
	// �e���[�N�̃N���A
	ZeroMemory(&m_mouseState, sizeof DIMOUSESTATE2);
	ZeroMemory(&m_mouseStateTrigger, sizeof DIMOUSESTATE2);
	ZeroMemory(&m_mouseStateRelease, sizeof DIMOUSESTATE2);
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

	// �f�o�C�X�̐ݒ�    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
	m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

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

	if (!m_pDIDevice) { return; }

	hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	
	if (SUCCEEDED(hr))
	{
		//Button�X�V
		for (int nCntButton = 0; nCntButton < sizeof(dims.rgbButtons); nCntButton++)
		{
			m_mouseStateTrigger.rgbButtons[nCntButton] = (m_mouseState.rgbButtons[nCntButton] ^ dims.rgbButtons[nCntButton]) & dims.rgbButtons[nCntButton];
			m_mouseStateRelease.rgbButtons[nCntButton] = (m_mouseState.rgbButtons[nCntButton] ^ dims.rgbButtons[nCntButton]) & ~dims.rgbButtons[nCntButton];
		}

		//�}�E�X�ʒu�X�V
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
// �L�[�{�[�h�f�[�^�擾(�v���X)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMousePress(int nButton)
{
	return (m_mouseState.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�g���K�[)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseTrigger(int nButton)
{
	return (m_mouseStateTrigger.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(�����[�X)
//--------------------------------------------------------------------------------
BOOL CMouseDX::GetMouseRelease(int nButton)
{
	return (m_mouseStateRelease.rgbButtons[nButton] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h�f�[�^�擾(���s�[�g)
//--------------------------------------------------------------------------------
//BOOL CMouseDX::GetMouseRepeat(int nButton)
//{
//	return (m_aMouseStateRepeat[nButton] & 0x80) ? TRUE : FALSE;
//}

//--------------------------------------------------------------------------------
// �}�E�X�f�[�^�폜(�g���K�[)
//--------------------------------------------------------------------------------
void CMouseDX::FlushMouseTrigger(int nButton)
{
	m_mouseStateTrigger.rgbButtons[nButton] = 0;
}

//--------------------------------------------------------------------------------
// �}�E�XX���擾
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisX(void)
{
	return m_mouseState.lX;
}

//--------------------------------------------------------------------------------
// �}�E�XY���擾
//--------------------------------------------------------------------------------
LONG CMouseDX::GetMouseAxisY(void)
{
	return m_mouseState.lY;
}

//--------------------------------------------------------------------------------
// �}�E�XZ���擾(Wheel)
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
// �R���X�g���X�^
//--------------------------------------------------------------------------------
CJoystickDX::CJoystickDX() : CInputDX()
	, m_hWnd(NULL)
	, m_bAttached(false)
{
	// �e���[�N�̃N���A
	ZeroMemory(&m_joyState, sizeof DIJOYSTATE2);
	ZeroMemory(&m_joyStateTrigger, sizeof DIJOYSTATE2);
	ZeroMemory(&m_joyStateRelease, sizeof DIJOYSTATE2);
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�̏���������
//--------------------------------------------------------------------------------
HRESULT CJoystickDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	m_hWnd = hWnd;

	// ���͏����̏�����
	hr = CInputDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// Joystick�f�o�C�X���쐬
	hr = AttachJoystick();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// �}�E�X�̍X�V����
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

	// ��Ԃ��擾����O�Ƀf�[�^���|�[�����O
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

		//Button�X�V
		for (int nCntButton = 0; nCntButton < sizeof(diJoy.rgbButtons); nCntButton++)
		{
			m_joyStateTrigger.rgbButtons[nCntButton] = (m_joyState.rgbButtons[nCntButton] ^ diJoy.rgbButtons[nCntButton]) & diJoy.rgbButtons[nCntButton];
			m_joyStateRelease.rgbButtons[nCntButton] = (m_joyState.rgbButtons[nCntButton] ^ diJoy.rgbButtons[nCntButton]) & ~diJoy.rgbButtons[nCntButton];
		}

		//LStick�X�V
		m_joyStateTrigger.lX = (m_joyState.lX ^ diJoy.lX) & diJoy.lX;
		m_joyStateRelease.lX = (m_joyState.lX ^ diJoy.lX) & ~diJoy.lX;
											  			  
		m_joyStateTrigger.lY = (m_joyState.lY ^ diJoy.lY) & diJoy.lY;
		m_joyStateRelease.lY = (m_joyState.lY ^ diJoy.lY) & ~diJoy.lY;
											  			  
		m_joyStateTrigger.lZ = (m_joyState.lZ ^ diJoy.lZ) & diJoy.lZ;
		m_joyStateRelease.lZ = (m_joyState.lZ ^ diJoy.lZ) & ~diJoy.lZ;

		//RStick�X�V
		m_joyStateTrigger.lRx = (m_joyState.lRx ^ diJoy.lRx) & diJoy.lRx;
		m_joyStateRelease.lRx = (m_joyState.lRx ^ diJoy.lRx) & ~diJoy.lRx;
															 
		m_joyStateTrigger.lRy = (m_joyState.lRy ^ diJoy.lRy) & diJoy.lRy;
		m_joyStateRelease.lRy = (m_joyState.lRy ^ diJoy.lRy) & ~diJoy.lRy;
															 
		m_joyStateTrigger.lRz = (m_joyState.lRz ^ diJoy.lRz) & diJoy.lRz;
		m_joyStateRelease.lRz = (m_joyState.lRz ^ diJoy.lRz) & ~diJoy.lRz;

		//2�̒ǉ���
		for (int nCntSlider = 0; nCntSlider < 2; nCntSlider++)
		{
			//�L�[�g���K�[�A�����[�X���̎擾
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
//  �񋓂��ꂽ�f�o�C�X���ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK CJoystickDX::EnumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext)
{
	DIDEVCAPS diDevCaps;				// Joystick�̔\�͏��
	HRESULT hr;
	LPDIRECTINPUTDEVICE8* ppDevice = (LPDIRECTINPUTDEVICE8*)pContext;

	//�񋓂��ꂽJoystick�ւ̃C���^�[�t�F�C�X���擾����
	hr = m_pDInput->CreateDevice(pDidInstance->guidInstance, ppDevice, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//Joystick�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = (*ppDevice)->GetCapabilities(&diDevCaps);
	if (FAILED(hr))
	{
		//Joystick�\�͂̎擾�Ɏ��s
		(*ppDevice)->Release();
		(*ppDevice) = NULL;
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//���̃f�o�C�X���g��
	return DIENUM_STOP;//�񋓂��I������ꍇ
					   //return DIENUM_CONTINUE;//����Joystick���T���ꍇ
}

//--------------------------------------------------------------------------------
//  �����ƂɌĂяo�����R�[���o�b�N�֐�
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
// �W���C�X�e�B�b�N�{�^���f�[�^�擾(�v���X)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonPress(const XBOX_BUTTON& button)
{
	return (m_joyState.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�{�^���f�[�^�擾(�g���K�[)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonTrigger(const XBOX_BUTTON& button)
{
	return (m_joyStateTrigger.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�{�^���f�[�^�擾(�����[�X)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetButtonRelease(const XBOX_BUTTON& button)
{
	return (m_joyStateRelease.rgbButtons[button] & 0x80) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�\���L�[�f�[�^�擾(�v���X)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyPress(const XBOX_CROSS_KEY& key)
{
	return (m_joyState.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�\���L�[�f�[�^�擾(�g���K�[)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyTrigger(const XBOX_CROSS_KEY& key)
{
	return (m_joyStateTrigger.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�\���L�[�f�[�^�擾(�����[�X)
//--------------------------------------------------------------------------------
BOOL CJoystickDX::GetCrossKeyRelease(const XBOX_CROSS_KEY& key)
{
	return (m_joyStateRelease.rgdwPOV[0] == key) ? TRUE : FALSE;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NLStickX�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLStickAxisX(void)
{
	return m_joyState.lX;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NLStickY�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLStickAxisY(void)
{
	return m_joyState.lY;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NLT&RT�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetLTandRT(void)
{
	return m_joyState.lZ;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NRStickX�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRStickAxisX(void)
{
	return m_joyState.lRx;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NRStickY�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRStickAxisY(void)
{
	return m_joyState.lRy;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�NRudder(������)�f�[�^�擾
//--------------------------------------------------------------------------------
LONG CJoystickDX::GetRudder(void)
{
	return m_joyState.lRz;
}

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�ƘA������A����������f�o�C�X�����
//--------------------------------------------------------------------------------
HRESULT CJoystickDX::AttachJoystick(void)
{
	HRESULT hr;

	// Joystick�f�o�C�X��񋓂��č쐬
	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, &m_pDIDevice, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		m_bAttached = false;
		//MessageBox(hWnd, "Joystick�f�o�C�X��񋓂��č쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystick�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystick�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// ���̒l�͈̔͂̐ݒ�
	hr = m_pDIDevice->EnumObjects(EnumAxesCallback, &m_pDIDevice, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, "Joystick�̎��̒l�͈̔͂�ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	m_bAttached = true;

	return hr;
}
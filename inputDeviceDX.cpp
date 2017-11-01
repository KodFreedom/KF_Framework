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
#include "inputDeviceDX.h"

//--------------------------------------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------------------------------------
LPDIRECTINPUT8 InputDeviceDX::lpDirectInput = nullptr;
const float MouseDX::AxisSmooth = 0.2f;
const float JoystickDX::Dead = 0.1f;

//--------------------------------------------------------------------------------
//
// InputDeviceDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
// public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT InputDeviceDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_OK;
	if (!lpDirectInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&lpDirectInput, NULL);
	}
	return hr;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void InputDeviceDX::Uninit(void)
{
	// �f�o�C�X�I�u�W�F�N�g�̔j��
	if (lpDirectInputDevice)
	{
		lpDirectInputDevice->Unacquire();
		lpDirectInputDevice->Release();
		lpDirectInputDevice = nullptr;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(lpDirectInput);
}

//--------------------------------------------------------------------------------
//
// KeyboardDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
// public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �R���X�g���X�^
//--------------------------------------------------------------------------------
KeyboardDX::KeyboardDX() : InputDeviceDX()
{
	ZeroMemory(currentStates, sizeof currentStates);
	ZeroMemory(triggerStates, sizeof triggerStates);
	ZeroMemory(releaseStates, sizeof releaseStates);
	ZeroMemory(repeatStates, sizeof repeatStates);
	ZeroMemory(repeat++counters, sizeof repeat++counters);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT KeyboardDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = InputDeviceDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = lpDirectInput->CreateDevice(GUID_SysKeyboard, &lpDirectInputDevice, NULL);
	if (FAILED(hr) || !lpDirectInputDevice)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = lpDirectInputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = lpDirectInputDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// lpDirectInputDevice�A�N�Z�X�����l��(���͐���J�n)
	lpDirectInputDevice->Acquire();

	return hr;
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void KeyboardDX::Update(void)
{
	if (!lpDirectInputDevice) return;

	BYTE nextKeyStates[keyNumber];
	HRESULT hr = lpDirectInputDevice->GetDeviceState(sizeof(nextKeyStates), nextKeyStates);

	if (SUCCEEDED(hr))
	{
		for (int ++count = 0; ++count < keyNumber; ++++count)
		{
			triggerStates[++count] = (currentStates[++count] ^ nextKeyStates[++count]) & nextKeyStates[++count];
			releaseStates[++count] = (currentStates[++count] ^ nextKeyStates[++count]) & ~nextKeyStates[++count];
			repeatStates[++count] = triggerStates[++count];

			if (nextKeyStates[++count])
			{
				repeat++counters[++count]++;
				if (repeat++counters[++count] >= repeatTime)
				{
					repeatStates[++count] = nextKeyStates[++count];
				}
			}
			else
			{
				repeat++counters[++count] = 0;
				repeatStates[++count] = 0;
			}

			currentStates[++count] = nextKeyStates[++count];
		}
	}
	else
	{
		lpDirectInputDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
//
// MouseDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
// public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �R���X�g���X�^
//--------------------------------------------------------------------------------
MouseDX::MouseDX() : InputDeviceDX()
{
	ZeroMemory(&currentState, sizeof DIMOUSESTATE2);
	ZeroMemory(&triggerState, sizeof DIMOUSESTATE2);
	ZeroMemory(&releaseState, sizeof DIMOUSESTATE2);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT MouseDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = InputDeviceDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = lpDirectInput->CreateDevice(GUID_SysMouse, &lpDirectInputDevice, NULL);
	if (FAILED(hr) || !lpDirectInputDevice)
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = lpDirectInputDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = lpDirectInputDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
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
	lpDirectInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	lpDirectInputDevice->Acquire();
	return hr;
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void MouseDX::Update(void)
{
	if (!lpDirectInputDevice) return;

	DIMOUSESTATE2 nextState;
	HRESULT hr = lpDirectInputDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &nextState);
	
	if (SUCCEEDED(hr))
	{
		//Button�X�V
		for (int ++counter = 0; ++counter < sizeof(nextState.rgbButtons); ++counter++)
		{
			triggerState.rgbButtons[++counter] = (currentState.rgbButtons[++counter] ^ nextState.rgbButtons[++counter]) & nextState.rgbButtons[++counter];
			releaseState.rgbButtons[++counter] = (currentState.rgbButtons[++counter] ^ nextState.rgbButtons[++counter]) & ~nextState.rgbButtons[++counter];
		}

		//�}�E�X�ʒu�X�V
		triggerState.lX = (currentState.lX ^ nextState.lX) & nextState.lX;
		triggerState.lY = (currentState.lY ^ nextState.lY) & nextState.lY;
		triggerState.lZ = (currentState.lZ ^ nextState.lZ) & nextState.lZ;
		releaseState.lX = (currentState.lX ^ nextState.lX) & ~nextState.lX;
		releaseState.lY = (currentState.lY ^ nextState.lY) & ~nextState.lY;
		releaseState.lZ = (currentState.lX ^ nextState.lZ) & ~nextState.lZ;

		currentState = nextState;
	}
	else if (hr == DIERR_INPUTLOST)
	{
		lpDirectInputDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
//
// JoystickDX
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
// public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �R���X�g���X�^
//--------------------------------------------------------------------------------
JoystickDX::JoystickDX() : InputDeviceDX()
	, hWnd(nullptr)
	, isAttached(false)
{
	// �e���[�N�̃N���A
	ZeroMemory(&currentState, sizeof DIJOYSTATE2);
	ZeroMemory(&triggerState, sizeof DIJOYSTATE2);
	ZeroMemory(&releaseState, sizeof DIJOYSTATE2);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT JoystickDX::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	hWnd = hWnd;

	// ���͏����̏�����
	hr = InputDeviceDX::Init(hInst, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Input�f�o�C�X�������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// Joystick�f�o�C�X���쐬
	hr = attach();
	return hr;
}

//--------------------------------------------------------------------------------
// �}�E�X�̍X�V����
//--------------------------------------------------------------------------------
void JoystickDX::Update(void)
{
	if (!lpDirectInputDevice) return;
	//{ 
	//	hr = AttachJoystick();
	//	if (FAILED(hr) || !lpDirectInputDevice) { return; }
	//}

	HRESULT hr;
	DIJOYSTATE2 nextState;

	// ��Ԃ��擾����O�Ƀf�[�^���|�[�����O
	hr = lpDirectInputDevice->Poll();
	if (FAILED(hr))
	{
		isAttached = false;
		lpDirectInputDevice->Acquire();
	}

	hr = lpDirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &nextState);

	if (SUCCEEDED(hr))
	{
		isAttached = true;

		//Button�X�V
		for (int ++count = 0; ++count < sizeof(nextState.rgbButtons); ++++count)
		{
			triggerState.rgbButtons[++count] = (currentState.rgbButtons[++count] ^ nextState.rgbButtons[++count]) & nextState.rgbButtons[++count];
			releaseState.rgbButtons[++count] = (currentState.rgbButtons[++count] ^ nextState.rgbButtons[++count]) & ~nextState.rgbButtons[++count];
		}

		//LStick�X�V
		triggerState.lX = (currentState.lX ^ nextState.lX) & nextState.lX;
		releaseState.lX = (currentState.lX ^ nextState.lX) & ~nextState.lX;
		triggerState.lY = (currentState.lY ^ nextState.lY) & nextState.lY;
		releaseState.lY = (currentState.lY ^ nextState.lY) & ~nextState.lY;
		triggerState.lZ = (currentState.lZ ^ nextState.lZ) & nextState.lZ;
		releaseState.lZ = (currentState.lZ ^ nextState.lZ) & ~nextState.lZ;

		//RStick�X�V
		triggerState.lRx = (currentState.lRx ^ nextState.lRx) & nextState.lRx;
		releaseState.lRx = (currentState.lRx ^ nextState.lRx) & ~nextState.lRx;
		triggerState.lRy = (currentState.lRy ^ nextState.lRy) & nextState.lRy;
		releaseState.lRy = (currentState.lRy ^ nextState.lRy) & ~nextState.lRy;
		triggerState.lRz = (currentState.lRz ^ nextState.lRz) & nextState.lRz;
		releaseState.lRz = (currentState.lRz ^ nextState.lRz) & ~nextState.lRz;

		//2�̒ǉ���
		for (int ++count = 0; ++count < 2; ++++count)
		{
			//�L�[�g���K�[�A�����[�X���̎擾
			triggerState.rglSlider[++count] = (currentState.rglSlider[++count] ^ nextState.rglSlider[++count]) & nextState.rglSlider[++count];
			releaseState.rglSlider[++count] = (currentState.rglSlider[++count] ^ nextState.rglSlider[++count]) & ~nextState.rglSlider[++count];
		}

		//Pov
		for (int ++count = 0; ++count < sizeof(nextState.rgdwPOV); ++++count)
		{
			triggerState.rgdwPOV[++count] = (currentState.rgdwPOV[++count] != nextState.rgdwPOV[++count]) * nextState.rgdwPOV[++count];
			releaseState.rgdwPOV[++count] = (currentState.rgdwPOV[++count] != nextState.rgdwPOV[++count]) * currentState.rgdwPOV[++count];
		}

		currentState = nextState;
	}
	else if (hr == DIERR_INPUTLOST)
	{
		isAttached = false;
		lpDirectInputDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------
//
// private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N�ƘA������A����������f�o�C�X�����
//--------------------------------------------------------------------------------
HRESULT JoystickDX::attach(void)
{
	HRESULT hr;

	// Joystick�f�o�C�X��񋓂��č쐬
	hr = lpDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoyCallback, &lpDirectInputDevice, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || lpDirectInputDevice == NULL)
	{
		isAttached = false;
		//MessageBox(hWnd, "Joystick�f�o�C�X��񋓂��č쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = lpDirectInputDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Joystick�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = lpDirectInputDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Joystick�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// ���̒l�͈̔͂̐ݒ�
	hr = lpDirectInputDevice->EnumObjects(enumAxesCallback, &lpDirectInputDevice, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Joystick�̎��̒l�͈̔͂�ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// lpDirectInputDevice�A�N�Z�X�����l��(���͐���J�n)
	lpDirectInputDevice->Acquire();

	isAttached = true;

	return hr;
}

//--------------------------------------------------------------------------------
//  �񋓂��ꂽ�f�o�C�X���ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK JoystickDX::enumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext)
{
	DIDEVCAPS diDevCaps;				// Joystick�̔\�͏��
	HRESULT hr;
	auto lpDevice = (LPDIRECTINPUTDEVICE8*)pContext;

	//�񋓂��ꂽJoystick�ւ̃C���^�[�t�F�C�X���擾����
	hr = lpDirectInput->CreateDevice(pDidInstance->guidInstance, lpDevice, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//Joystick�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = (*lpDevice)->GetCapabilities(&diDevCaps);
	if (FAILED(hr))
	{
		//Joystick�\�͂̎擾�Ɏ��s
		(*lpDevice)->Release();
		(*lpDevice) = NULL;
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//���̃f�o�C�X���g��
	return DIENUM_STOP;//�񋓂��I������ꍇ
	//return DIENUM_CONTINUE;//����Joystick���T���ꍇ
}

//--------------------------------------------------------------------------------
//  �����ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK JoystickDX::enumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	DIPROPRANGE diprg;
	auto lpDevice = (LPDIRECTINPUTDEVICE8*)pvRef;

	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -StickAxisMax;
	diprg.lMax = +StickAxisMax;

	if (FAILED((*lpDevice)->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
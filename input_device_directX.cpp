//--------------------------------------------------------------------------------
//�@input_device_directX.cpp
//  directInput�̃f�B�o�C�X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "input_device_directX.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------------------------------------
LPDIRECTINPUT8 InputDeviceDirectX::instance_ = nullptr;

//--------------------------------------------------------------------------------
//
// InputDeviceDirectX
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
HRESULT InputDeviceDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT hr = S_OK;
    if (!instance_)
    {
        hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
            IID_IDirectInput8, (void**)&instance_, NULL);
    }
    return hr;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void InputDeviceDirectX::Uninit(void)
{
    // �f�o�C�X�I�u�W�F�N�g�̔j��
    if (device_)
    {
        device_->Unacquire();
        device_->Release();
        device_ = nullptr;
    }

    // DirectInput�I�u�W�F�N�g�̔j��
    SAFE_RELEASE(instance_);
}

//--------------------------------------------------------------------------------
//
// KeyboardDirectX
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
KeyboardDirectX::KeyboardDirectX() : InputDeviceDirectX()
{
    ZeroMemory(current_states_, sizeof current_states_);
    ZeroMemory(trigger_states_, sizeof trigger_states_);
    ZeroMemory(release_states_, sizeof release_states_);
    ZeroMemory(repeat_states_, sizeof repeat_states_);
    ZeroMemory(repeat_counters_, sizeof repeat_counters_);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT KeyboardDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Input�f�o�C�X�������Ɏ��s�I", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�o�C�X�I�u�W�F�N�g���쐬
    hr = instance_->CreateDevice(GUID_SysKeyboard, &device_, NULL);
    if (FAILED(hr) || !device_)
    {
        MessageBox(NULL, L"�L�[�{�[�h������܂���I", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�[�^�t�H�[�}�b�g��ݒ�
    hr = device_->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
    hr = device_->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // device_�A�N�Z�X�����l��(���͐���J�n)
    device_->Acquire();

    return hr;
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void KeyboardDirectX::Update(void)
{
    if (!device_) return;
    BYTE next_key_states[kKeyNumber];
    if (SUCCEEDED(device_->GetDeviceState(sizeof(next_key_states), next_key_states)))
    {
        for (int count = 0; count < kKeyNumber; ++count)
        {
            trigger_states_[count] = (current_states_[count] ^ next_key_states[count]) & next_key_states[count];
            release_states_[count] = (current_states_[count] ^ next_key_states[count]) & ~next_key_states[count];
            repeat_states_[count] = trigger_states_[count];
            if (next_key_states[count])
            {
                repeat_counters_[count]++;
                if (repeat_counters_[count] >= kRepeatTime)
                {
                    repeat_states_[count] = next_key_states[count];
                }
            }
            else
            {
                repeat_counters_[count] = 0;
                repeat_states_[count] = 0;
            }
            current_states_[count] = next_key_states[count];
        }
    }
    else
    {
        device_->Acquire();
    }
}

//--------------------------------------------------------------------------------
//
// MouseDirectX
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
MouseDirectX::MouseDirectX() : InputDeviceDirectX()
{
    ZeroMemory(&current_state_, sizeof current_state_);
    ZeroMemory(&trigger_state_, sizeof trigger_state_);
    ZeroMemory(&release_state_, sizeof release_state_);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT MouseDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    // ���͏����̏�����
    HRESULT hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Input�f�o�C�X�������Ɏ��s�I", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�o�C�X�I�u�W�F�N�g���쐬
    hr = instance_->CreateDevice(GUID_SysMouse, &device_, NULL);
    if (FAILED(hr) || !device_)
    {
        MessageBox(NULL, L"�}�E�X���˂��I", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�[�^�t�H�[�}�b�g��ݒ�
    hr = device_->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
    hr = device_->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�o�C�X�̐ݒ�    
    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
    device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);
    device_->Acquire();
    return hr;
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void MouseDirectX::Update(void)
{
    if (!device_) return;
    DIMOUSESTATE2 next_state;
    HRESULT hr = device_->GetDeviceState(sizeof(DIMOUSESTATE2), &next_state);
    if (SUCCEEDED(hr))
    {
        //Button�X�V
        for (int counter = 0; counter < sizeof(next_state.rgbButtons); counter++)
        {
            trigger_state_.rgbButtons[counter] = (current_state_.rgbButtons[counter] ^ next_state.rgbButtons[counter]) & next_state.rgbButtons[counter];
            release_state_.rgbButtons[counter] = (current_state_.rgbButtons[counter] ^ next_state.rgbButtons[counter]) & ~next_state.rgbButtons[counter];
        }

        //�}�E�X�ʒu�X�V
        trigger_state_.lX = (current_state_.lX ^ next_state.lX) & next_state.lX;
        trigger_state_.lY = (current_state_.lY ^ next_state.lY) & next_state.lY;
        trigger_state_.lZ = (current_state_.lZ ^ next_state.lZ) & next_state.lZ;
        release_state_.lX = (current_state_.lX ^ next_state.lX) & ~next_state.lX;
        release_state_.lY = (current_state_.lY ^ next_state.lY) & ~next_state.lY;
        release_state_.lZ = (current_state_.lX ^ next_state.lZ) & ~next_state.lZ;

        current_state_ = next_state;
    }
    else if (hr == DIERR_INPUTLOST)
    {
        device_->Acquire();
    }
}

//--------------------------------------------------------------------------------
//
// JoystickDirectX
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
JoystickDirectX::JoystickDirectX() : InputDeviceDirectX()
    , hwnd_(nullptr), is_attached_(false)
{
    // �e���[�N�̃N���A
    ZeroMemory(&current_state_, sizeof DIJOYSTATE2);
    ZeroMemory(&trigger_state_, sizeof DIJOYSTATE2);
    ZeroMemory(&release_state_, sizeof DIJOYSTATE2);
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT JoystickDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT hr;
    hwnd_ = hwnd;

    // ���͏����̏�����
    hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Input�f�o�C�X�������Ɏ��s�I", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // Joystick�f�o�C�X���쐬
    hr = Attach();
    return hr;
}

//--------------------------------------------------------------------------------
// �}�E�X�̍X�V����
//--------------------------------------------------------------------------------
void JoystickDirectX::Update(void)
{
    if (!device_) return;
    //{ 
    //    hr = AttachJoystick();
    //    if (FAILED(hr) || !device_) { return; }
    //}

    HRESULT hr;
    DIJOYSTATE2 next_state;

    // ��Ԃ��擾����O�Ƀf�[�^���|�[�����O
    hr = device_->Poll();
    if (FAILED(hr))
    {
        is_attached_ = false;
        device_->Acquire();
    }

    hr = device_->GetDeviceState(sizeof(DIJOYSTATE2), &next_state);

    if (SUCCEEDED(hr))
    {
        is_attached_ = true;

        //Button�X�V
        for (int count = 0; count < sizeof(next_state.rgbButtons); ++count)
        {
            trigger_state_.rgbButtons[count] = (current_state_.rgbButtons[count] ^ next_state.rgbButtons[count]) & next_state.rgbButtons[count];
            release_state_.rgbButtons[count] = (current_state_.rgbButtons[count] ^ next_state.rgbButtons[count]) & ~next_state.rgbButtons[count];
        }

        //LStick�X�V
        trigger_state_.lX = (current_state_.lX ^ next_state.lX) & next_state.lX;
        release_state_.lX = (current_state_.lX ^ next_state.lX) & ~next_state.lX;
        trigger_state_.lY = (current_state_.lY ^ next_state.lY) & next_state.lY;
        release_state_.lY = (current_state_.lY ^ next_state.lY) & ~next_state.lY;
        trigger_state_.lZ = (current_state_.lZ ^ next_state.lZ) & next_state.lZ;
        release_state_.lZ = (current_state_.lZ ^ next_state.lZ) & ~next_state.lZ;

        //RStick�X�V
        trigger_state_.lRx = (current_state_.lRx ^ next_state.lRx) & next_state.lRx;
        release_state_.lRx = (current_state_.lRx ^ next_state.lRx) & ~next_state.lRx;
        trigger_state_.lRy = (current_state_.lRy ^ next_state.lRy) & next_state.lRy;
        release_state_.lRy = (current_state_.lRy ^ next_state.lRy) & ~next_state.lRy;
        trigger_state_.lRz = (current_state_.lRz ^ next_state.lRz) & next_state.lRz;
        release_state_.lRz = (current_state_.lRz ^ next_state.lRz) & ~next_state.lRz;

        //2�̒ǉ���
        for (int count = 0; count < 2; ++count)
        {
            //�L�[�g���K�[�A�����[�X���̎擾
            trigger_state_.rglSlider[count] = (current_state_.rglSlider[count] ^ next_state.rglSlider[count]) & next_state.rglSlider[count];
            release_state_.rglSlider[count] = (current_state_.rglSlider[count] ^ next_state.rglSlider[count]) & ~next_state.rglSlider[count];
        }

        //Pov
        for (int count = 0; count < sizeof(next_state.rgdwPOV); ++count)
        {
            trigger_state_.rgdwPOV[count] = (current_state_.rgdwPOV[count] != next_state.rgdwPOV[count]) * next_state.rgdwPOV[count];
            release_state_.rgdwPOV[count] = (current_state_.rgdwPOV[count] != next_state.rgdwPOV[count]) * current_state_.rgdwPOV[count];
        }

        current_state_ = next_state;
    }
    else if (hr == DIERR_INPUTLOST)
    {
        is_attached_ = false;
        device_->Acquire();
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
HRESULT JoystickDirectX::Attach(void)
{
    HRESULT hr;

    // Joystick�f�o�C�X��񋓂��č쐬
    hr = instance_->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, &device_, DIEDFL_ATTACHEDONLY);
    if (FAILED(hr) || device_ == NULL)
    {
        is_attached_ = false;
        //MessageBox(NULL, "Joystick�f�o�C�X��񋓂��č쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);
        return hr;
    }

    // �f�[�^�t�H�[�}�b�g��ݒ�
    hr = device_->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystick�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
    hr = device_->SetCooperativeLevel(hwnd_, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystick�̋������[�h��ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return hr;
    }

    // ���̒l�͈̔͂̐ݒ�
    hr = device_->EnumObjects(EnumAxesCallback, &device_, DIDFT_AXIS);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystick�̎��̒l�͈̔͂�ݒ�ł��܂���ł����B", L"�x���I", MB_ICONWARNING);
        return E_FAIL;
    }

    // device_�A�N�Z�X�����l��(���͐���J�n)
    device_->Acquire();

    is_attached_ = true;

    return hr;
}

//--------------------------------------------------------------------------------
//  �񋓂��ꂽ�f�o�C�X���ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK JoystickDirectX::EnumJoyCallback(const DIDEVICEINSTANCE* instance, VOID *context)
{
    DIDEVCAPS didevcaps; // Joystick�̔\�͏��
    auto device_pointer = static_cast<LPDIRECTINPUTDEVICE8*>(context);

    //�񋓂��ꂽJoystick�ւ̃C���^�[�t�F�C�X���擾����
    HRESULT hr = instance_->CreateDevice(instance->guidInstance, device_pointer, NULL);
    if (FAILED(hr))
    {
        return DIENUM_CONTINUE;//�񋓂𑱂���
    }

    //Joystick�̔\�͂𒲂ׂ�
    didevcaps.dwSize = sizeof(DIDEVCAPS);
    hr = (*device_pointer)->GetCapabilities(&didevcaps);
    if (FAILED(hr))
    {
        //Joystick�\�͂̎擾�Ɏ��s
        (*device_pointer)->Release();
        (*device_pointer) = NULL;
        return DIENUM_CONTINUE;//�񋓂𑱂���
    }

    //���̃f�o�C�X���g��
    return DIENUM_STOP;//�񋓂��I������ꍇ
    //return DIENUM_CONTINUE;//����Joystick���T���ꍇ
}

//--------------------------------------------------------------------------------
//  �����ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK JoystickDirectX::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
    DIPROPRANGE diprg;
    auto device_pointer = static_cast<LPDIRECTINPUTDEVICE8*>(pvRef);
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj = lpddoi->dwType;
    diprg.diph.dwHow = DIPH_BYID;
    diprg.lMin = -static_cast<LONG>(kStickAxisMax);
    diprg.lMax = +static_cast<LONG>(kStickAxisMax);
    if (FAILED((*device_pointer)->SetProperty(DIPROP_RANGE, &diprg.diph)))
    {
        return DIENUM_STOP;
    }
    return DIENUM_CONTINUE;
}
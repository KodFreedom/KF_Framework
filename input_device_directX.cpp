//--------------------------------------------------------------------------------
//　input_device_directX.cpp
//  directInputのディバイス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "input_device_directX.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
// 静的変数
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
// 初期化処理
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
// 終了処理
//--------------------------------------------------------------------------------
void InputDeviceDirectX::Uninit(void)
{
    // デバイスオブジェクトの破棄
    if (device_)
    {
        device_->Unacquire();
        device_->Release();
        device_ = nullptr;
    }

    // DirectInputオブジェクトの破棄
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
// コンストラスタ
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
// 初期化処理
//--------------------------------------------------------------------------------
HRESULT KeyboardDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Inputデバイスが生成に失敗！", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスオブジェクトを作成
    hr = instance_->CreateDevice(GUID_SysKeyboard, &device_, NULL);
    if (FAILED(hr) || !device_)
    {
        MessageBox(NULL, L"キーボードがありません！", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = device_->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"キーボードのデータフォーマットを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = device_->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"キーボードの協調モードを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // device_アクセス権を獲得(入力制御開始)
    device_->Acquire();

    return hr;
}

//--------------------------------------------------------------------------------
// 更新処理
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
// コンストラスタ
//--------------------------------------------------------------------------------
MouseDirectX::MouseDirectX() : InputDeviceDirectX()
{
    ZeroMemory(&current_state_, sizeof current_state_);
    ZeroMemory(&trigger_state_, sizeof trigger_state_);
    ZeroMemory(&release_state_, sizeof release_state_);
}

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
HRESULT MouseDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    // 入力処理の初期化
    HRESULT hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Inputデバイスが生成に失敗！", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスオブジェクトを作成
    hr = instance_->CreateDevice(GUID_SysMouse, &device_, NULL);
    if (FAILED(hr) || !device_)
    {
        MessageBox(NULL, L"マウスがねぇ！", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = device_->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"キーボードのデータフォーマットを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = device_->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"マウスの協調モードを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスの設定    
    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
    device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);
    device_->Acquire();
    return hr;
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void MouseDirectX::Update(void)
{
    if (!device_) return;
    DIMOUSESTATE2 next_state;
    HRESULT hr = device_->GetDeviceState(sizeof(DIMOUSESTATE2), &next_state);
    if (SUCCEEDED(hr))
    {
        //Button更新
        for (int counter = 0; counter < sizeof(next_state.rgbButtons); counter++)
        {
            trigger_state_.rgbButtons[counter] = (current_state_.rgbButtons[counter] ^ next_state.rgbButtons[counter]) & next_state.rgbButtons[counter];
            release_state_.rgbButtons[counter] = (current_state_.rgbButtons[counter] ^ next_state.rgbButtons[counter]) & ~next_state.rgbButtons[counter];
        }

        //マウス位置更新
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
// コンストラスタ
//--------------------------------------------------------------------------------
JoystickDirectX::JoystickDirectX() : InputDeviceDirectX()
    , hwnd_(nullptr), is_attached_(false)
{
    // 各ワークのクリア
    ZeroMemory(&current_state_, sizeof DIJOYSTATE2);
    ZeroMemory(&trigger_state_, sizeof DIJOYSTATE2);
    ZeroMemory(&release_state_, sizeof DIJOYSTATE2);
}

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
HRESULT JoystickDirectX::Init(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT hr;
    hwnd_ = hwnd;

    // 入力処理の初期化
    hr = InputDeviceDirectX::Init(hinstance, hwnd);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Inputデバイスが生成に失敗！", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // Joystickデバイスを作成
    hr = Attach();
    return hr;
}

//--------------------------------------------------------------------------------
// マウスの更新処理
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

    // 状態を取得する前にデータをポーリング
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

        //Button更新
        for (int count = 0; count < sizeof(next_state.rgbButtons); ++count)
        {
            trigger_state_.rgbButtons[count] = (current_state_.rgbButtons[count] ^ next_state.rgbButtons[count]) & next_state.rgbButtons[count];
            release_state_.rgbButtons[count] = (current_state_.rgbButtons[count] ^ next_state.rgbButtons[count]) & ~next_state.rgbButtons[count];
        }

        //LStick更新
        trigger_state_.lX = (current_state_.lX ^ next_state.lX) & next_state.lX;
        release_state_.lX = (current_state_.lX ^ next_state.lX) & ~next_state.lX;
        trigger_state_.lY = (current_state_.lY ^ next_state.lY) & next_state.lY;
        release_state_.lY = (current_state_.lY ^ next_state.lY) & ~next_state.lY;
        trigger_state_.lZ = (current_state_.lZ ^ next_state.lZ) & next_state.lZ;
        release_state_.lZ = (current_state_.lZ ^ next_state.lZ) & ~next_state.lZ;

        //RStick更新
        trigger_state_.lRx = (current_state_.lRx ^ next_state.lRx) & next_state.lRx;
        release_state_.lRx = (current_state_.lRx ^ next_state.lRx) & ~next_state.lRx;
        trigger_state_.lRy = (current_state_.lRy ^ next_state.lRy) & next_state.lRy;
        release_state_.lRy = (current_state_.lRy ^ next_state.lRy) & ~next_state.lRy;
        trigger_state_.lRz = (current_state_.lRz ^ next_state.lRz) & next_state.lRz;
        release_state_.lRz = (current_state_.lRz ^ next_state.lRz) & ~next_state.lRz;

        //2つの追加軸
        for (int count = 0; count < 2; ++count)
        {
            //キートリガー、リリース情報の取得
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
// ジョイスティックと連結する、成功したらデバイスを作る
//--------------------------------------------------------------------------------
HRESULT JoystickDirectX::Attach(void)
{
    HRESULT hr;

    // Joystickデバイスを列挙して作成
    hr = instance_->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, &device_, DIEDFL_ATTACHEDONLY);
    if (FAILED(hr) || device_ == NULL)
    {
        is_attached_ = false;
        //MessageBox(NULL, "Joystickデバイスを列挙して作成に失敗！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = device_->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystickのデータフォーマットを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = device_->SetCooperativeLevel(hwnd_, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystickの協調モードを設定できませんでした。", L"警告！", MB_ICONWARNING);
        return hr;
    }

    // 軸の値の範囲の設定
    hr = device_->EnumObjects(EnumAxesCallback, &device_, DIDFT_AXIS);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Joystickの軸の値の範囲を設定できませんでした。", L"警告！", MB_ICONWARNING);
        return E_FAIL;
    }

    // device_アクセス権を獲得(入力制御開始)
    device_->Acquire();

    is_attached_ = true;

    return hr;
}

//--------------------------------------------------------------------------------
//  列挙されたデバイスごとに呼び出されるコールバック関数
//--------------------------------------------------------------------------------
BOOL CALLBACK JoystickDirectX::EnumJoyCallback(const DIDEVICEINSTANCE* instance, VOID *context)
{
    DIDEVCAPS didevcaps; // Joystickの能力情報
    auto device_pointer = static_cast<LPDIRECTINPUTDEVICE8*>(context);

    //列挙されたJoystickへのインターフェイスを取得する
    HRESULT hr = instance_->CreateDevice(instance->guidInstance, device_pointer, NULL);
    if (FAILED(hr))
    {
        return DIENUM_CONTINUE;//列挙を続ける
    }

    //Joystickの能力を調べる
    didevcaps.dwSize = sizeof(DIDEVCAPS);
    hr = (*device_pointer)->GetCapabilities(&didevcaps);
    if (FAILED(hr))
    {
        //Joystick能力の取得に失敗
        (*device_pointer)->Release();
        (*device_pointer) = NULL;
        return DIENUM_CONTINUE;//列挙を続ける
    }

    //このデバイスを使う
    return DIENUM_STOP;//列挙を終了する場合
    //return DIENUM_CONTINUE;//他のJoystickも探す場合
}

//--------------------------------------------------------------------------------
//  軸ごとに呼び出されるコールバック関数
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
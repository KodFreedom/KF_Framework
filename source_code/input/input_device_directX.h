//--------------------------------------------------------------------------------
//　input_device_directX.h
//  directInputのディバイス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

//--------------------------------------------------------------------------------
//  library / ライブラリ
//--------------------------------------------------------------------------------
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum MouseButton
{
    kMouseLeft = 0,
    kMouseRight,
    kMouseMiddle,
    kMouseMax
};

enum XboxButton
{
    kXboxA = 0,
    kXboxB = 1,
    kXboxX = 2,
    kXboxY = 3,
    kXboxLB = 4,
    kXboxRB = 5,
    kXboxBack = 6,
    kXboxMenu = 7,
    kXboxLS = 8,
    kXboxRS = 9
};

enum XboxCrossKey
{
    kXboxCrossKeyUp = 0,
    kXboxCrossKeyDown = 18000,
    kXboxCrossKeyLeft = 27000,
    kXboxCrossKeyRight = 9000
};

enum XboxStick
{
    kXboxStickUp = -1000,
    kXboxStickDown = 1000,
    kXboxStickLeft = -1000,
    kXboxStickRight = 1000
};

//--------------------------------------------------------------------------------
// 基底クラス
//--------------------------------------------------------------------------------
class InputDeviceDirectX
{
public:
    //--------------------------------------------------------------------------------
    // 初期化処理
    //--------------------------------------------------------------------------------
    virtual HRESULT Init(HINSTANCE hinstance, HWND hwnd);

    //--------------------------------------------------------------------------------
    // 終了処理
    //--------------------------------------------------------------------------------
    virtual void Uninit(void);

    //--------------------------------------------------------------------------------
    // 更新処理
    //--------------------------------------------------------------------------------
    virtual void Update(void) = 0;

    //--------------------------------------------------------------------------------
    // 所有権取得
    //--------------------------------------------------------------------------------
    void Acquire(void) { if (device_) { device_->Acquire(); } }

    //--------------------------------------------------------------------------------
    // 所有権放棄
    //--------------------------------------------------------------------------------
    void Unacquire(void) { if (device_) { device_->Unacquire(); } }

    //--------------------------------------------------------------------------------
    // 何かのキーが押されたフラグ
    //--------------------------------------------------------------------------------
    bool IsPressedAnyKey(void) const { return is_pressed_any_key_; }

protected:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr int kRepeatTime = 20;

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    InputDeviceDirectX() {}
    InputDeviceDirectX(const InputDeviceDirectX& value) {}
    InputDeviceDirectX& operator=(const InputDeviceDirectX& value) {}
    ~InputDeviceDirectX() {}

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    static LPDIRECTINPUT8 instance_; // DirectInputオブジェクト
    LPDIRECTINPUTDEVICE8  device_ = nullptr; // Deviceオブジェクト(入力に必要)
    bool                  is_pressed_any_key_ = false;
};

//--------------------------------------------------------------------------------
// キーボード入力クラス
//--------------------------------------------------------------------------------
class KeyboardDirectX : public InputDeviceDirectX
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    KeyboardDirectX();
    ~KeyboardDirectX() {}

    //--------------------------------------------------------------------------------
    // 初期化処理
    //--------------------------------------------------------------------------------
    HRESULT Init(HINSTANCE hinstance, HWND hwnd) override;

    //--------------------------------------------------------------------------------
    // 更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    // キーのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetPress(const int key) const { return (current_states_[key] & 0x80) ? TRUE : FALSE; }

    //--------------------------------------------------------------------------------
    // キーのトリガーの取得
    //--------------------------------------------------------------------------------
    BOOL GetTrigger(const int key) const { return (trigger_states_[key] & 0x80) ? TRUE : FALSE; }

    //--------------------------------------------------------------------------------
    // キーのリリースの取得
    //--------------------------------------------------------------------------------
    BOOL GetRelease(const int key) const { return (release_states_[key] & 0x80) ? TRUE : FALSE; }

    //--------------------------------------------------------------------------------
    // キーのリピートの取得
    //--------------------------------------------------------------------------------
    BOOL GetRepeat(const int key) const { return (repeat_states_[key] & 0x80) ? TRUE : FALSE; }
    
private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr int kKeyNumber = 88; // f12まで

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    BYTE current_states_[kKeyNumber];
    BYTE trigger_states_[kKeyNumber];
    BYTE release_states_[kKeyNumber];
    BYTE repeat_states_[kKeyNumber];
    int  repeat_counters_[kKeyNumber];
};

//--------------------------------------------------------------------------------
// マウス入力クラス
//--------------------------------------------------------------------------------
class MouseDirectX : public InputDeviceDirectX
{
public:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kAxisMax = 120.0f;
    static constexpr float kAxisSmooth = 0.2f;

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    MouseDirectX();
    ~MouseDirectX() {}

    //--------------------------------------------------------------------------------
    // 初期化処理
    //--------------------------------------------------------------------------------
    HRESULT Init(HINSTANCE hinstance, HWND hwnd) override;

    //--------------------------------------------------------------------------------
    // 更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    // マウスボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetPress(const MouseButton& button) const { return (current_state_.rgbButtons[static_cast<int>(button)] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // マウスボタンのトリガーの取得
    //--------------------------------------------------------------------------------
    BOOL GetTrigger(const MouseButton& button) const { return (trigger_state_.rgbButtons[static_cast<int>(button)] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // マウスボタンのリリースの取得
    //--------------------------------------------------------------------------------
    BOOL GetRelease(const MouseButton& button) const { return (release_state_.rgbButtons[static_cast<int>(button)] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // マウスX軸の取得
    //--------------------------------------------------------------------------------
    LONG GetAxisX(void) const { return current_state_.lX; }

    //--------------------------------------------------------------------------------
    // マウスY軸の取得
    //--------------------------------------------------------------------------------
    LONG GetAxisY(void) const { return current_state_.lY; }

    //--------------------------------------------------------------------------------
    // マウスZ軸の取得
    //--------------------------------------------------------------------------------
    LONG GetAxisZ(void) const { return current_state_.lZ; }

private:
    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    DIMOUSESTATE2 current_state_;
    DIMOUSESTATE2 trigger_state_;
    DIMOUSESTATE2 release_state_;
};

//--------------------------------------------------------------------------------
// ジョイスティック入力クラス
//--------------------------------------------------------------------------------
class JoystickDirectX : public InputDeviceDirectX
{
public:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kStickAxisMax = 1000.0f;
    static constexpr float kDead = 0.1f;

    //--------------------------------------------------------------------------------
    //  関数定義
    //--------------------------------------------------------------------------------
    JoystickDirectX();
    ~JoystickDirectX() {}

    //--------------------------------------------------------------------------------
    // 初期化処理
    //--------------------------------------------------------------------------------
    HRESULT Init(HINSTANCE hinstance, HWND hwnd) override;

    //--------------------------------------------------------------------------------
    // 更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetButtonPress(const XboxButton& button) const { return (current_state_.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetButtonTrigger(const XboxButton& button) const { return (trigger_state_.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetButtonRelease(const XboxButton& button) const { return (release_state_.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetCrossKeyPress(const XboxCrossKey& key) const { return (current_state_.rgdwPOV[0] == key) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetCrossKeyTrigger(const XboxCrossKey& key) const { return(trigger_state_.rgdwPOV[0] == key) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    BOOL GetCrossKeyRelease(const XboxCrossKey& key) const { return (release_state_.rgdwPOV[0] == key) ? TRUE : FALSE; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetLStickAxisX(void) const { return current_state_.lX; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetLStickAxisY(void) const { return current_state_.lY; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetLTandRT(void) const { return current_state_.lZ; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetRStickAxisX(void) const { return current_state_.lRx; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetRStickAxisY(void) const { return current_state_.lRy; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    LONG GetRudder(void) const { return current_state_.lRz; }
    
    //--------------------------------------------------------------------------------
    // ボタンのプレスの取得
    //--------------------------------------------------------------------------------
    bool IsAttached(void) const { return is_attached_; }

private:
    //--------------------------------------------------------------------------------
    // ジョイスティックと連結する、成功したらデバイスを作る
    //--------------------------------------------------------------------------------
    HRESULT    Attach(void);

    //--------------------------------------------------------------------------------
    //  列挙されたデバイスごとに呼び出されるコールバック関数
    //--------------------------------------------------------------------------------
    static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* instance, VOID *context);
    
    //--------------------------------------------------------------------------------
    //  軸ごとに呼び出されるコールバック関数
    //--------------------------------------------------------------------------------
    static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE instance, LPVOID ref);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    DIJOYSTATE2 current_state_;
    DIJOYSTATE2 trigger_state_;
    DIJOYSTATE2 release_state_;
    HWND        hwnd_;
    bool        is_attached_;
};
//--------------------------------------------------------------------------------
//　input.cpp
//  入力ディバイス情報管理
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "input.h"
#include "input_device_directX.h"
#include "kf_math.h"
#include "kf_utility.h"
using namespace kodfreedom;
#include <cereal/archives/binary.hpp>
using namespace cereal;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Input::Update(void)
{
    keyboard_->Update();
    mouse_->Update();
    joystick_->Update();

    if (is_demo_play_)
    {
        LoadInputInfo();
    }
    else
    {
        UpdateInputInfo();
        
        if (is_save_demo_play_)
        {
            SaveInputInfo();
        }
    }
}

//--------------------------------------------------------------------------------
//  使用権取得
//--------------------------------------------------------------------------------
void Input::Acquire(void)
{
    keyboard_->Acquire();
    mouse_->Acquire();
    joystick_->Acquire();
}

//--------------------------------------------------------------------------------
//  使用権解放
//--------------------------------------------------------------------------------
void Input::Unacquire(void)
{
    keyboard_->Unacquire();
    mouse_->Unacquire();
    joystick_->Unacquire();
}

//--------------------------------------------------------------------------------
// 何かのキーが押されたフラグ
//--------------------------------------------------------------------------------
bool Input::IsPressedAnyKey(void) const
{
    return keyboard_->IsPressedAnyKey() | mouse_->IsPressedAnyKey() | joystick_->IsPressedAnyKey();
}

//--------------------------------------------------------------------------------
//  デモプレイモードの設定
//--------------------------------------------------------------------------------
void Input::SetDemoPlayMode(const bool& enable)
{
    is_demo_play_ = enable;
    if (is_demo_play_)
    {
        if (!file_for_load_.is_open())
        {
            key_info_.end_of_file = 0;
            file_for_load_.open(L"data/input/demo_play.log", ios::binary);
            assert(file_for_load_.is_open());
        }
    }
    else
    {
        if (file_for_load_.is_open())
        {
            file_for_load_.close();
        }
    }
}

//--------------------------------------------------------------------------------
//  デモプレイセーブの設定
//--------------------------------------------------------------------------------
void Input::SetSaveDemoPlay(const bool& enable)
{
    is_save_demo_play_ = enable;
    if (is_save_demo_play_)
    {
        if (!file_for_save_.is_open())
        {
            file_for_save_.open(L"data/input/demo_play.log", ios::binary);
            assert(file_for_save_.is_open());
            file_for_save_.clear();
        }
    }
    else
    {
        if (file_for_save_.is_open())
        {
            key_info_.end_of_file = 1;
            BinaryOutputArchive archive(file_for_save_);
            archive.saveBinary(&key_info_, sizeof(key_info_));
            file_for_save_.close();
        }
    }
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Input::Input()
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool Input::Init(HINSTANCE hinstance, HWND hwnd)
{
    keyboard_ = MY_NEW KeyboardDirectX;
    keyboard_->Init(hinstance, hwnd);
    mouse_ = MY_NEW MouseDirectX;
    mouse_->Init(hinstance, hwnd);
    joystick_ = MY_NEW JoystickDirectX;
    joystick_->Init(hinstance, hwnd);
    return true;
}

//--------------------------------------------------------------------------------
//  入力情報更新処理
//--------------------------------------------------------------------------------
void Input::UpdateInputInfo(void)
{
    bool press_flags[kKeyMax] = { 0 };
    bool trigger_flags[kKeyMax] = { 0 };
    bool release_flags[kKeyMax] = { 0 };

    // Move
    float keyboard_axis_x = -static_cast<float>(keyboard_->GetPress(DIK_A))
        + static_cast<float>(keyboard_->GetPress(DIK_D));
    float keyboard_axis_y = static_cast<float>(keyboard_->GetPress(DIK_W))
        - static_cast<float>(keyboard_->GetPress(DIK_S));
    float joystick_left_axis_x = static_cast<float>(joystick_->GetLStickAxisX()) / JoystickDirectX::kStickAxisMax;
    float joystick_left_axis_y = -static_cast<float>(joystick_->GetLStickAxisY()) / JoystickDirectX::kStickAxisMax;
    joystick_left_axis_x = fabsf(joystick_left_axis_x) > JoystickDirectX::kDead ? joystick_left_axis_x : 0.0f;
    joystick_left_axis_y = fabsf(joystick_left_axis_y) > JoystickDirectX::kDead ? joystick_left_axis_y : 0.0f;
    key_info_.move_horizontal = Math::AbsMax(keyboard_axis_x, joystick_left_axis_x);
    key_info_.move_vertical = Math::AbsMax(keyboard_axis_y, joystick_left_axis_y);

    // Rotation
    float rotation_axis_x = 0.0f;
    float rotation_axis_y = 0.0f;
    if (is_editor_mode_)
    {
        rotation_axis_x = static_cast<float>(mouse_->GetAxisX()) * MouseDirectX::kAxisSmooth;
        rotation_axis_y = static_cast<float>(mouse_->GetAxisY()) * MouseDirectX::kAxisSmooth;
    }
    else
    {
        rotation_axis_x = -static_cast<float>(keyboard_->GetPress(DIK_Q)) + static_cast<float>(keyboard_->GetPress(DIK_E));
        rotation_axis_y = -static_cast<float>(keyboard_->GetPress(DIK_R)) + static_cast<float>(keyboard_->GetPress(DIK_T));
    }
    float joystick_right_axis_x = static_cast<float>(joystick_->GetRStickAxisX()) / JoystickDirectX::kStickAxisMax;
    float joystick_right_axis_y = static_cast<float>(joystick_->GetRStickAxisY()) / JoystickDirectX::kStickAxisMax;
    key_info_.rotation_horizontal = Math::AbsMax(rotation_axis_x, joystick_right_axis_x);
    key_info_.rotation_vertical = Math::AbsMax(rotation_axis_y, joystick_right_axis_y);

    // zoom_
    float mouse_axis_z = -static_cast<float>(mouse_->GetAxisZ()) / MouseDirectX::kAxisMax;
    float joystick_axis_z = static_cast<float>(joystick_->GetLTandRT()) / JoystickDirectX::kStickAxisMax;
    key_info_.zoom = Math::AbsMax(mouse_axis_z, joystick_axis_z);

    // Key
    // Submit
    press_flags[kSubmit] = keyboard_->GetPress(DIK_SPACE) | joystick_->GetButtonPress(kXboxA);
    trigger_flags[kSubmit] = keyboard_->GetTrigger(DIK_SPACE) | joystick_->GetButtonTrigger(kXboxA);
    release_flags[kSubmit] = keyboard_->GetRelease(DIK_SPACE) | joystick_->GetButtonRelease(kXboxA);

    // Cancel
    press_flags[kCancel] = keyboard_->GetPress(DIK_J) | joystick_->GetButtonPress(kXboxB);
    trigger_flags[kCancel] = keyboard_->GetTrigger(DIK_J) | joystick_->GetButtonTrigger(kXboxB);
    release_flags[kCancel] = keyboard_->GetRelease(DIK_J) | joystick_->GetButtonRelease(kXboxB);

    // Start
    press_flags[kStart] = keyboard_->GetPress(DIK_RETURN) | joystick_->GetButtonPress(kXboxMenu);
    trigger_flags[kStart] = keyboard_->GetTrigger(DIK_RETURN) | joystick_->GetButtonTrigger(kXboxMenu);
    release_flags[kStart] = keyboard_->GetRelease(DIK_RETURN) | joystick_->GetButtonRelease(kXboxMenu);

    // Up
    press_flags[kUp] = keyboard_->GetPress(DIK_UP);// | joystick_->GetCrossKeyPress(kXboxCrossKeyUp);
    trigger_flags[kUp] = keyboard_->GetTrigger(DIK_UP);// | joystick_->GetCrossKeyTrigger(kXboxCrossKeyUp);
    release_flags[kUp] = keyboard_->GetRelease(DIK_UP);// | joystick_->GetCrossKeyRelease(kXboxCrossKeyUp);

    // Down
    press_flags[kDown] = keyboard_->GetPress(DIK_DOWN) | joystick_->GetCrossKeyPress(kXboxCrossKeyDown);
    trigger_flags[kDown] = keyboard_->GetTrigger(DIK_DOWN) | joystick_->GetCrossKeyTrigger(kXboxCrossKeyDown);
    release_flags[kDown] = keyboard_->GetRelease(DIK_DOWN) | joystick_->GetCrossKeyRelease(kXboxCrossKeyDown);

    // Left
    press_flags[kLeft] = keyboard_->GetPress(DIK_LEFT) | joystick_->GetCrossKeyPress(kXboxCrossKeyLeft);
    trigger_flags[kLeft] = keyboard_->GetTrigger(DIK_LEFT) | joystick_->GetCrossKeyTrigger(kXboxCrossKeyLeft);
    release_flags[kLeft] = keyboard_->GetRelease(DIK_LEFT) | joystick_->GetCrossKeyRelease(kXboxCrossKeyLeft);

    // Right
    press_flags[kRight] = keyboard_->GetPress(DIK_RIGHT) | joystick_->GetCrossKeyPress(kXboxCrossKeyRight);
    trigger_flags[kRight] = keyboard_->GetTrigger(DIK_RIGHT) | joystick_->GetCrossKeyTrigger(kXboxCrossKeyRight);
    release_flags[kRight] = keyboard_->GetRelease(DIK_RIGHT) | joystick_->GetCrossKeyRelease(kXboxCrossKeyRight);

    // Reset
    press_flags[kReset] = keyboard_->GetPress(DIK_R);
    trigger_flags[kReset] = keyboard_->GetTrigger(DIK_R);
    release_flags[kReset] = keyboard_->GetRelease(DIK_R);

    // Lock
    press_flags[kLock] = mouse_->GetPress(kMouseRight);
    trigger_flags[kLock] = mouse_->GetTrigger(kMouseRight);
    release_flags[kLock] = mouse_->GetRelease(kMouseRight);

    // StrongAttack
    press_flags[kStrongAttack] = keyboard_->GetPress(DIK_K) | joystick_->GetButtonPress(kXboxX);
    trigger_flags[kStrongAttack] = keyboard_->GetTrigger(DIK_K) | joystick_->GetButtonTrigger(kXboxX);
    release_flags[kStrongAttack] = keyboard_->GetRelease(DIK_K) | joystick_->GetButtonRelease(kXboxX);

    // Skill
    press_flags[kSkill] = keyboard_->GetPress(DIK_L) | joystick_->GetButtonPress(kXboxY);
    trigger_flags[kSkill] = keyboard_->GetTrigger(DIK_L) | joystick_->GetButtonTrigger(kXboxY);
    release_flags[kSkill] = keyboard_->GetRelease(DIK_L) | joystick_->GetButtonRelease(kXboxY);

    // Defence
    press_flags[kBlock] = keyboard_->GetPress(DIK_LSHIFT) | joystick_->GetButtonPress(kXboxLB);
    trigger_flags[kBlock] = keyboard_->GetTrigger(DIK_LSHIFT) | joystick_->GetButtonTrigger(kXboxLB);
    release_flags[kBlock] = keyboard_->GetRelease(DIK_LSHIFT) | joystick_->GetButtonRelease(kXboxLB);

    // 保存
    key_info_.press_state = 0;
    key_info_.trigger_state = 0;
    key_info_.release_state = 0;
    for (int count = 0; count < kKeyMax; ++count)
    {
        key_info_.press_state |= press_flags[count] << count;
        key_info_.trigger_state |= trigger_flags[count] << count;
        key_info_.release_state |= release_flags[count] << count;
    }
}

//--------------------------------------------------------------------------------
//  入力情報の保存
//--------------------------------------------------------------------------------
void Input::SaveInputInfo(void)
{
    if (!file_for_save_.is_open())
    {// open file
        file_for_save_.open(L"data/input/demo_play.log", ios::binary);
        assert(file_for_save_.is_open());
    }

    BinaryOutputArchive archive(file_for_save_);
    archive.saveBinary(&key_info_, sizeof(key_info_));
}

//--------------------------------------------------------------------------------
//  入力情報の読込
//--------------------------------------------------------------------------------
void Input::LoadInputInfo(void)
{
    assert(file_for_load_.is_open());
    if (key_info_.end_of_file == 1) { return; }

    BinaryInputArchive archive(file_for_load_);
    archive.loadBinary(&key_info_, sizeof(key_info_));
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Input::Uninit(void)
{
    SAFE_UNINIT(keyboard_);
    SAFE_UNINIT(mouse_);
    SAFE_UNINIT(joystick_);

    if (file_for_save_.is_open())
    {
        key_info_.end_of_file = 1;
        BinaryOutputArchive archive(file_for_save_);
        archive.saveBinary(&key_info_, sizeof(key_info_));
        file_for_save_.close();
    }

    if (file_for_load_.is_open())
    {
        file_for_load_.close();
    }
}
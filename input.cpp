//--------------------------------------------------------------------------------
//　input.cpp
//	入力ディバイス情報管理
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "input.h"
#include "input_device_directX.h"
#include "kf_math.h"
#include "kf_utility.h"
using namespace kodfreedom;

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
	UpdateInputInfo();
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
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Input::Input()
	: keyboard_(nullptr)
	, mouse_(nullptr)
	, joystick_(nullptr)
	, move_horizontal_(0.0f)
	, move_vertical_(0.0f)
	, rotation_horizontal_(0.0f)
	, rotation_vertical_(0.0f)
	, zoom_(0.0f)
	, press_state_(0)
	, trigger_state_(0)
	, release_state_(0)
	, is_editor_mode_(false)
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
	bool press_flags[static_cast<int>(kKeyMax)] = { 0 };
	bool trigger_flags[static_cast<int>(kKeyMax)] = { 0 };
	bool release_flags[static_cast<int>(kKeyMax)] = { 0 };

	// Move
	float keyboard_axis_x = -static_cast<float>(keyboard_->GetPress(DIK_A))
		+ static_cast<float>(keyboard_->GetPress(DIK_D));
	float keyboard_axis_y = static_cast<float>(keyboard_->GetPress(DIK_W))
		- static_cast<float>(keyboard_->GetPress(DIK_S));
	float joystick_left_axis_x = static_cast<float>(joystick_->GetLStickAxisX()) / JoystickDirectX::kStickAxisMax;
	float joystick_left_axis_y = -static_cast<float>(joystick_->GetLStickAxisY()) / JoystickDirectX::kStickAxisMax;
	joystick_left_axis_x = fabsf(joystick_left_axis_x) > JoystickDirectX::kDead ? joystick_left_axis_x : 0.0f;
	joystick_left_axis_y = fabsf(joystick_left_axis_y) > JoystickDirectX::kDead ? joystick_left_axis_y : 0.0f;
	move_horizontal_ = Math::AbsMax(keyboard_axis_x, joystick_left_axis_x);
	move_vertical_ = Math::AbsMax(keyboard_axis_y, joystick_left_axis_y);

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
	rotation_horizontal_ = Math::AbsMax(rotation_axis_x, joystick_right_axis_x);
	rotation_vertical_ = Math::AbsMax(rotation_axis_y, joystick_right_axis_y);

	// zoom_
	float mouse_axis_z = -static_cast<float>(mouse_->GetAxisZ()) / MouseDirectX::kAxisMax;
	float joystick_axis_z = static_cast<float>(joystick_->GetLTandRT()) / JoystickDirectX::kStickAxisMax;
	zoom_ = Math::AbsMax(mouse_axis_z, joystick_axis_z);

	// Key
	// Submit
	press_flags[static_cast<int>(Key::kSubmit)] = keyboard_->GetPress(DIK_SPACE) | joystick_->GetButtonPress(XboxButton::kXboxA);
	trigger_flags[static_cast<int>(Key::kSubmit)] = keyboard_->GetTrigger(DIK_SPACE) | joystick_->GetButtonTrigger(XboxButton::kXboxA);
	release_flags[static_cast<int>(Key::kSubmit)] = keyboard_->GetRelease(DIK_SPACE) | joystick_->GetButtonRelease(XboxButton::kXboxA);

	// Cancel
	press_flags[static_cast<int>(Key::kCancel)] = keyboard_->GetPress(DIK_J) | joystick_->GetButtonPress(XboxButton::kXboxB);
	trigger_flags[static_cast<int>(Key::kCancel)] = keyboard_->GetTrigger(DIK_J) | joystick_->GetButtonTrigger(XboxButton::kXboxB);
	release_flags[static_cast<int>(Key::kCancel)] = keyboard_->GetRelease(DIK_J) | joystick_->GetButtonRelease(XboxButton::kXboxB);

	// Start
	press_flags[static_cast<int>(Key::kStart)] = keyboard_->GetPress(DIK_RETURN) | joystick_->GetButtonPress(XboxButton::kXboxMenu);
	trigger_flags[static_cast<int>(Key::kStart)] = keyboard_->GetTrigger(DIK_RETURN) | joystick_->GetButtonTrigger(XboxButton::kXboxMenu);
	release_flags[static_cast<int>(Key::kStart)] = keyboard_->GetRelease(DIK_RETURN) | joystick_->GetButtonRelease(XboxButton::kXboxMenu);

	// Up
	press_flags[static_cast<int>(Key::kUp)] = keyboard_->GetPress(DIK_UP);
	trigger_flags[static_cast<int>(Key::kUp)] = keyboard_->GetTrigger(DIK_UP);
	release_flags[static_cast<int>(Key::kUp)] = keyboard_->GetRelease(DIK_UP);

	// Down
	press_flags[static_cast<int>(Key::kDown)] = keyboard_->GetPress(DIK_DOWN);
	trigger_flags[static_cast<int>(Key::kDown)] = keyboard_->GetTrigger(DIK_DOWN);
	release_flags[static_cast<int>(Key::kDown)] = keyboard_->GetRelease(DIK_DOWN);

	// Left
	press_flags[static_cast<int>(Key::kLeft)] = keyboard_->GetPress(DIK_LEFT);
	trigger_flags[static_cast<int>(Key::kLeft)] = keyboard_->GetTrigger(DIK_LEFT);
	release_flags[static_cast<int>(Key::kLeft)] = keyboard_->GetRelease(DIK_LEFT);

	// Right
	press_flags[static_cast<int>(Key::kRight)] = keyboard_->GetPress(DIK_RIGHT);
	trigger_flags[static_cast<int>(Key::kRight)] = keyboard_->GetTrigger(DIK_RIGHT);
	release_flags[static_cast<int>(Key::kRight)] = keyboard_->GetRelease(DIK_RIGHT);

	// Reset
	press_flags[static_cast<int>(Key::kReset)] = keyboard_->GetPress(DIK_R);
	trigger_flags[static_cast<int>(Key::kReset)] = keyboard_->GetTrigger(DIK_R);
	release_flags[static_cast<int>(Key::kReset)] = keyboard_->GetRelease(DIK_R);

	// Lock
	press_flags[static_cast<int>(Key::kLock)] = mouse_->GetPress(MouseButton::kMouseRight);
	trigger_flags[static_cast<int>(Key::kLock)] = mouse_->GetTrigger(MouseButton::kMouseRight);
	release_flags[static_cast<int>(Key::kLock)] = mouse_->GetRelease(MouseButton::kMouseRight);

	press_state_ = 0;
	trigger_state_ = 0;
	release_state_ = 0;
	for (int count = 0; count < static_cast<int>(kKeyMax); ++count)
	{
		press_state_ |= press_flags[count] << count;
		trigger_state_ |= trigger_flags[count] << count;
		release_state_ |= release_flags[count] << count;
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Input::Uninit(void)
{
	SAFE_UNINIT(keyboard_);
	SAFE_UNINIT(mouse_);
	SAFE_UNINIT(joystick_);
}
//--------------------------------------------------------------------------------
//	入力情報管理処理
//　input.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "inputDeviceDX.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
Input* Input::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：Create
//  関数説明：生成処理
//	引数：	hInstance：値
//			hWnd：
//			isWindowMode：
//	戻り値：Manager*
//--------------------------------------------------------------------------------
Input* Input::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (instance) return instance;
	instance = new Input;
	instance->init(hInstance, hWnd);
	return instance;
}

//--------------------------------------------------------------------------------
//	関数名：Release
//  関数説明：破棄処理
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Input::Release(void)
{
	SAFE_UNINIT(instance);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Input::Update(void)
{
	keyboard->Update();
	mouse->Update();
	joystick->Update();
	updateInputInfo();
}

//--------------------------------------------------------------------------------
//  使用権取得
//--------------------------------------------------------------------------------
void Input::Acquire(void)
{
	keyboard->Acquire();
	mouse->Acquire();
	joystick->Acquire();
}

//--------------------------------------------------------------------------------
//  使用権解放
//--------------------------------------------------------------------------------
void Input::Unacquire(void)
{
	keyboard->Unacquire();
	mouse->Unacquire();
	joystick->Unacquire();
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
	: keyboard(nullptr)
	, mouse(nullptr)
	, joystick(nullptr)
	, moveHorizontal(0.0f)
	, moveVertical(0.0f)
	, rotHorizontal(0.0f)
	, rotVertical(0.0f)
	, zoom(0.0f)
	, pressInfos(0)
	, triggerInfos(0)
	, releaseInfos(0)
	, isEditorMode(false)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool Input::init(HINSTANCE hInst, HWND hWnd)
{
	keyboard = new KeyboardDX;
	keyboard->Init(hInst, hWnd);
	mouse = new MouseDX;
	mouse->Init(hInst, hWnd);
	joystick = new JoystickDX;
	joystick->Init(hInst, hWnd);
	return true;
}

//--------------------------------------------------------------------------------
//  入力情報更新処理
//--------------------------------------------------------------------------------
void Input::updateInputInfo(void)
{
	bool pressFlags[(int)Key::Max] = { 0 };
	bool triggerFlags[(int)Key::Max] = { 0 };
	bool releaseFlags[(int)Key::Max] = { 0 };

	// Move
	float keyboardAxisX = -(float)keyboard->GetPress(DIK_A) + (float)keyboard->GetPress(DIK_D);
	float keyboardAxisY = (float)keyboard->GetPress(DIK_W) - (float)keyboard->GetPress(DIK_S);
	float joystickLeftAxisX = (float)joystick->GetLStickAxisX() / JoystickDX::StickAxisMax;
	float joystickLeftAxisY = -(float)joystick->GetLStickAxisY() / JoystickDX::StickAxisMax;
	joystickLeftAxisX = fabsf(joystickLeftAxisX) > JoystickDX::Dead ? joystickLeftAxisX : 0.0f;
	joystickLeftAxisY = fabsf(joystickLeftAxisY) > JoystickDX::Dead ? joystickLeftAxisY : 0.0f;
	moveHorizontal = Math::AbsMax(keyboardAxisX, joystickLeftAxisX);
	moveVertical = Math::AbsMax(keyboardAxisY, joystickLeftAxisY);

	// Rotation
	float rotationAxisX = 0.0f;
	float rotationAxisY = 0.0f;
	if (isEditorMode)
	{
		rotationAxisX = (float)mouse->GetAxisX() * MouseDX::AxisSmooth;
		rotationAxisY = (float)mouse->GetAxisY() * MouseDX::AxisSmooth;
	}
	else
	{
		rotationAxisX = -(float)keyboard->GetPress(DIK_Q) + (float)keyboard->GetPress(DIK_E);
		rotationAxisY = -(float)keyboard->GetPress(DIK_R) + (float)keyboard->GetPress(DIK_T);
	}
	float joystickRightAxisX = (float)joystick->GetRStickAxisX() / JoystickDX::StickAxisMax;
	float joystickRightAxisY = (float)joystick->GetRStickAxisY() / JoystickDX::StickAxisMax;
	rotHorizontal = Math::AbsMax(rotationAxisX, joystickRightAxisX);
	rotVertical = Math::AbsMax(rotationAxisY, joystickRightAxisY);

	// zoom
	float mouseAxisZ = -(float)mouse->GetAxisZ() / MouseDX::AxisMax;
	float joystickAxisZ = (float)joystick->GetLTandRT() / JoystickDX::StickAxisMax;
	zoom = Math::AbsMax(mouseAxisZ, joystickAxisZ);

	// Key
	// Submit
	pressFlags[(int)Key::Submit] = keyboard->GetPress(DIK_SPACE) | joystick->GetButtonPress(XboxButton::A);
	triggerFlags[(int)Key::Submit] = keyboard->GetTrigger(DIK_SPACE) | joystick->GetButtonTrigger(XboxButton::A);
	releaseFlags[(int)Key::Submit] = keyboard->GetRelease(DIK_SPACE) | joystick->GetButtonRelease(XboxButton::A);

	// Cancel
	pressFlags[(int)Key::Cancel] = keyboard->GetPress(DIK_J) | joystick->GetButtonPress(XboxButton::B);
	triggerFlags[(int)Key::Cancel] = keyboard->GetTrigger(DIK_J) | joystick->GetButtonTrigger(XboxButton::B);
	releaseFlags[(int)Key::Cancel] = keyboard->GetRelease(DIK_J) | joystick->GetButtonRelease(XboxButton::B);

	// Start
	pressFlags[(int)Key::Start] = keyboard->GetPress(DIK_RETURN) | joystick->GetButtonPress(XboxButton::Menu);
	triggerFlags[(int)Key::Start] = keyboard->GetTrigger(DIK_RETURN) | joystick->GetButtonTrigger(XboxButton::Menu);
	releaseFlags[(int)Key::Start] = keyboard->GetRelease(DIK_RETURN) | joystick->GetButtonRelease(XboxButton::Menu);

	// RotX
	pressFlags[(int)Key::RotationXMinus] = keyboard->GetPress(DIK_T);
	triggerFlags[(int)Key::RotationXMinus] = keyboard->GetTrigger(DIK_T);
	releaseFlags[(int)Key::RotationXMinus] = keyboard->GetRelease(DIK_T);
	pressFlags[(int)Key::RotationXPlus] = keyboard->GetPress(DIK_Y);
	triggerFlags[(int)Key::RotationXPlus] = keyboard->GetTrigger(DIK_Y);
	releaseFlags[(int)Key::RotationXPlus] = keyboard->GetRelease(DIK_Y);

	// RotY
	pressFlags[(int)Key::RotationYMinus] = keyboard->GetPress(DIK_G);
	triggerFlags[(int)Key::RotationYMinus] = keyboard->GetTrigger(DIK_G);
	releaseFlags[(int)Key::RotationYMinus] = keyboard->GetRelease(DIK_G);
	pressFlags[(int)Key::RotationYPlus] = keyboard->GetPress(DIK_H);
	triggerFlags[(int)Key::RotationYPlus] = keyboard->GetTrigger(DIK_H);
	releaseFlags[(int)Key::RotationYPlus] = keyboard->GetRelease(DIK_H);

	// RotZ
	pressFlags[(int)Key::RotationZMinus] = keyboard->GetPress(DIK_B);
	triggerFlags[(int)Key::RotationZMinus] = keyboard->GetTrigger(DIK_B);
	releaseFlags[(int)Key::RotationZMinus] = keyboard->GetRelease(DIK_B);
	pressFlags[(int)Key::RotationZPlus] = keyboard->GetPress(DIK_N);
	triggerFlags[(int)Key::RotationZPlus] = keyboard->GetTrigger(DIK_N);
	releaseFlags[(int)Key::RotationZPlus] = keyboard->GetRelease(DIK_N);

	// Up
	pressFlags[(int)Key::Up] = keyboard->GetPress(DIK_UP);
	triggerFlags[(int)Key::Up] = keyboard->GetTrigger(DIK_UP);
	releaseFlags[(int)Key::Up] = keyboard->GetRelease(DIK_UP);

	// Down
	pressFlags[(int)Key::Down] = keyboard->GetPress(DIK_DOWN);
	triggerFlags[(int)Key::Down] = keyboard->GetTrigger(DIK_DOWN);
	releaseFlags[(int)Key::Down] = keyboard->GetRelease(DIK_DOWN);

	// Left
	pressFlags[(int)Key::Left] = keyboard->GetPress(DIK_LEFT);
	triggerFlags[(int)Key::Left] = keyboard->GetTrigger(DIK_LEFT);
	releaseFlags[(int)Key::Left] = keyboard->GetRelease(DIK_LEFT);

	// Right
	pressFlags[(int)Key::Right] = keyboard->GetPress(DIK_RIGHT);
	triggerFlags[(int)Key::Right] = keyboard->GetTrigger(DIK_RIGHT);
	releaseFlags[(int)Key::Right] = keyboard->GetRelease(DIK_RIGHT);

	//LeftBracket
	pressFlags[(int)Key::LeftBracket] = keyboard->GetPress(DIK_LBRACKET);
	triggerFlags[(int)Key::LeftBracket] = keyboard->GetTrigger(DIK_LBRACKET);
	releaseFlags[(int)Key::LeftBracket] = keyboard->GetRelease(DIK_LBRACKET);

	//RightBracket
	pressFlags[(int)Key::RightBracket] = keyboard->GetPress(DIK_RBRACKET);
	triggerFlags[(int)Key::RightBracket] = keyboard->GetTrigger(DIK_RBRACKET);
	releaseFlags[(int)Key::RightBracket] = keyboard->GetRelease(DIK_RBRACKET);

	//Reset
	pressFlags[(int)Key::Reset] = keyboard->GetPress(DIK_R);
	triggerFlags[(int)Key::Reset] = keyboard->GetTrigger(DIK_R);
	releaseFlags[(int)Key::Reset] = keyboard->GetRelease(DIK_R);

	pressInfos = 0;
	triggerInfos = 0;
	releaseInfos = 0;
	for (int count = 0; count < (int)Key::Max; ++count)
	{
		pressInfos |= pressFlags[count] << count;
		triggerInfos |= triggerFlags[count] << count;
		releaseInfos |= releaseFlags[count] << count;
	}

	/*
	pressInfos =
	(LONG)bSubmitPress << Submit
	| (bCancelPress << Cancel)
	| (bStartPress << Start)
	| (bRotXMinusPress << RotationXMinus)
	| (bRotXPlusPress << RotationXPlus)
	| (bRotYMinusPress << RotationYMinus)
	| (bRotYPlusPress << RotationYPlus)
	| (bRotZMinusPress << RotationZMinus)
	| (bRotZPlusPress << RotationZPlus)
	| (bUpPress << Up)
	| (bDownPress << Down)
	| (bLeftPress << Left)
	| (bRightPress << Right)
	| (bLRacketPress << LeftBracket)
	| (bRRacketPress << RightBracket)
	| (bResetPress << Reset);

	triggerInfos =
	(LONG)bSubmitTrigger << Submit
	| (bCancelTrigger << Cancel)
	| (bStartTrigger << Start)
	| (bRotXMinusTrigger << RotationXMinus)
	| (bRotXPlusTrigger << RotationXPlus)
	| (bRotYMinusTrigger << RotationYMinus)
	| (bRotYPlusTrigger << RotationYPlus)
	| (bRotZMinusTrigger << RotationZMinus)
	| (bRotZPlusTrigger << RotationZPlus)
	| (bUpTrigger << Up)
	| (bDownTrigger << Down)
	| (bLeftTrigger << Left)
	| (bRightTrigger << Right)
	| (bLRacketTrigger << LeftBracket)
	| (bRRacketTrigger << RightBracket)
	| (bResetTrigger << Reset);

	releaseInfos =
	(LONG)bSubmitRelease << Submit
	| (bCancelRelease << Cancel)
	| (bStartRelease << Start)
	| (bRotXMinusRelease << RotationXMinus)
	| (bRotXPlusRelease << RotationXPlus)
	| (bRotYMinusRelease << RotationYMinus)
	| (bRotYPlusRelease << RotationYPlus)
	| (bRotZMinusRelease << RotationZMinus)
	| (bRotZPlusRelease << RotationZPlus)
	| (bUpRelease << Up)
	| (bDownRelease << Down)
	| (bLeftRelease << Left)
	| (bRightRelease << Right)
	| (bLRacketRelease << LeftBracket)
	| (bRRacketRelease << RightBracket)
	| (bResetRelease << Reset);
	*/
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Input::uninit(void)
{
	if (keyboard)
	{
		keyboard->Uninit();
		delete keyboard;
		keyboard = nullptr;
	}

	if (mouse)
	{
		mouse->Uninit();
		delete mouse;
		mouse = nullptr;
	}

	if (joystick)
	{
		joystick->Uninit();
		delete joystick;
		joystick = nullptr;
	}
}
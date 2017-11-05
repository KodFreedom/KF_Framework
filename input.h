//--------------------------------------------------------------------------------
//	入力情報管理処理
//　input.h
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class KeyboardDX;
class MouseDX;
class JoystickDX;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum Key
{
	Submit = 0,
	Cancel,
	Start,
	Up,
	Down,
	Left,
	Right,
	Reset,
	Lock,
	Max,

	//重ねるキー
	Jump = Submit,
	Attack = Cancel,
	Save = Start,
	Extend = Right,
	Shrink = Left,
	Raise = Up,
	Reduce = Down,
};

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class Input
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static Input*	Create(HINSTANCE hInstance, HWND hWnd);
	static void		Release(void);
	static auto		Instance(void) { return instance; }
	void			Update(void);
	void			Acquire(void);
	void			Unacquire(void);
	void			SetEditorMode(const bool& enable) { isEditorMode = enable; }
	auto			Keyboard(void) { return keyboard; }
	auto			Mouse(void) { return mouse; }
	auto			Joystick(void) { return joystick; }
	float			MoveHorizontal(void) { return moveHorizontal; }
	float			MoveVertical(void) { return moveVertical; }
	float			RotHorizontal(void) { return rotHorizontal; }
	float			RotVertical(void) { return rotVertical; }
	float			Zoom(void) { return zoom; }
	bool			GetKeyPress(const Key& key) const { return pressInfos & (1 << (int)key); }
	bool			GetKeyTrigger(const Key& key) const { return triggerInfos & (1 << (int)key); }
	bool			GetKeyRelease(const Key& key) const { return releaseInfos & (1 << (int)key); }

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Input();
	~Input() {}
	bool			init(HINSTANCE hInst, HWND hWnd);
	void			uninit(void);
	void			updateInputInfo(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	KeyboardDX*		keyboard;
	MouseDX*		mouse;
	JoystickDX*		joystick;
	float			moveHorizontal;
	float			moveVertical;	
	float			rotHorizontal;	
	float			rotVertical;	
	float			zoom;			
	LONG			pressInfos;		
	LONG			triggerInfos;	
	LONG			releaseInfos;	
	bool			isEditorMode;
	static Input*	instance;
};
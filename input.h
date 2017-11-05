//--------------------------------------------------------------------------------
//	���͏��Ǘ�����
//�@input.h
//	Author : Xu Wenjie
//	Date   : 2017-06-21
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class KeyboardDX;
class MouseDX;
class JoystickDX;

//--------------------------------------------------------------------------------
//  �񋓌^��`
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

	//�d�˂�L�[
	Jump = Submit,
	Attack = Cancel,
	Save = Start,
	Extend = Right,
	Shrink = Left,
	Raise = Up,
	Reduce = Down,
};

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
class Input
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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
	//  �֐���`
	//--------------------------------------------------------------------------------
	Input();
	~Input() {}
	bool			init(HINSTANCE hInst, HWND hWnd);
	void			uninit(void);
	void			updateInputInfo(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
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
//--------------------------------------------------------------------------------
//	DirectX���͏���
//�@inputDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum MouseButton
{
	Left = 0,
	Right,
	Middle,
	ButtonMax
};

enum XboxButton
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LB = 4,
	RB = 5,
	Back = 6,
	Menu = 7,
	LS = 8,
	RS = 9
};

enum XboxCrossKey
{
	Up = 0,
	Down = 18000,
	Left = 27000,
	Right = 9000
};

enum XboxStick
{
	Up = -1000,
	Down = 1000,
	Left = -1000,
	Right = 1000
};

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// ���̓N���X
//--------------------------------------------------------------------------------
class InputDeviceDX
{
public:
	InputDeviceDX() : lpDirectInputDevice(nullptr) {}
	virtual ~InputDeviceDX() {}

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void	Uninit(void);
	virtual void	Update(void) = 0;

	void Acquire(void) { if (lpDirectInputDevice) { lpDirectInputDevice->Acquire(); } }
	void Unacquire(void) { if (lpDirectInputDevice) { lpDirectInputDevice->Unacquire(); } }

protected:
	static const int		repeatTime = 20;
	static LPDIRECTINPUT8	lpDirectInput;			// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	lpDirectInputDevice;	// Device�I�u�W�F�N�g(���͂ɕK�v)
};

//--------------------------------------------------------------------------------
// �L�[�{�[�h���̓N���X
//--------------------------------------------------------------------------------
class KeyboardDX : public InputDeviceDX
{
public:
	KeyboardDX();
	~KeyboardDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void	Update(void) override;
	BOOL	GetPress(int nKey) const { return (currentStates[nKey] & 0x80) ? TRUE : FALSE; }
	BOOL	GetTrigger(int nKey) const { return (triggerStates[nKey] & 0x80) ? TRUE : FALSE; }
	BOOL	GetRelease(int nKey) const { return (releaseStates[nKey] & 0x80) ? TRUE : FALSE; }
	BOOL	GetRepeat(int nKey) const { return (repeatStates[nKey] & 0x80) ? TRUE : FALSE; }
	
private:
	static const int keyNumber = 256;
	BYTE currentStates[keyNumber];
	BYTE triggerStates[keyNumber];
	BYTE releaseStates[keyNumber];
	BYTE repeatStates[keyNumber];
	int	 repeat++counters[keyNumber];
};

//--------------------------------------------------------------------------------
// �}�E�X���̓N���X
//--------------------------------------------------------------------------------
class MouseDX : public InputDeviceDX
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int AxisMax = 120;
	static const float AxisSmooth;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MouseDX();
	~MouseDX() {}

	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void	Update(void) override;
	BOOL	GetPress(int nButton) const { return (currentState.rgbButtons[nButton] & 0x80) ? TRUE : FALSE; }
	BOOL	GetTrigger(int nButton) const { return (triggerState.rgbButtons[nButton] & 0x80) ? TRUE : FALSE; }
	BOOL	GetRelease(int nButton) const { return (releaseState.rgbButtons[nButton] & 0x80) ? TRUE : FALSE; }
	LONG	GetAxisX(void) const { return currentState.lX; }
	LONG	GetAxisY(void) const { return currentState.lY; }
	LONG	GetAxisZ(void) const { return currentState.lZ; }

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	DIMOUSESTATE2 currentState;
	DIMOUSESTATE2 triggerState;
	DIMOUSESTATE2 releaseState;
};

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N���̓N���X
//--------------------------------------------------------------------------------
class JoystickDX : public InputDeviceDX
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int StickAxisMax = 1000;
	static const float Dead;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	JoystickDX();
	~JoystickDX() {}
	
	HRESULT Init(HINSTANCE hInst, HWND hWnd) override;
	void	Update(void) override;
	BOOL	GetButtonPress(const XboxButton& button) const { return (currentState.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
	BOOL	GetButtonTrigger(const XboxButton& button) const { return (triggerState.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
	BOOL	GetButtonRelease(const XboxButton& button) const { return (releaseState.rgbButtons[button] & 0x80) ? TRUE : FALSE; }
	BOOL	GetCrossKeyPress(const XboxCrossKey& key) const { return (currentState.rgdwPOV[0] == key) ? TRUE : FALSE; }
	BOOL	GetCrossKeyTrigger(const XboxCrossKey& key) const { return(triggerState.rgdwPOV[0] == key) ? TRUE : FALSE; }
	BOOL	GetCrossKeyRelease(const XboxCrossKey& key) const { return (releaseState.rgdwPOV[0] == key) ? TRUE : FALSE; }
	LONG	GetLStickAxisX(void) const { return currentState.lX; }
	LONG	GetLStickAxisY(void) const { return currentState.lY; }
	LONG	GetLTandRT(void) const { return currentState.lZ; }
	LONG	GetRStickAxisX(void) const { return currentState.lRx; }
	LONG	GetRStickAxisY(void) const { return currentState.lRy; }
	LONG	GetRudder(void) const { return currentState.lRz; }
	bool	IsAttached(void) const { return isAttached; }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	HRESULT	attach(void);
	static BOOL CALLBACK enumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext);
	static BOOL CALLBACK enumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	DIJOYSTATE2 currentState;
	DIJOYSTATE2 triggerState;
	DIJOYSTATE2 releaseState;
	HWND		hWnd;
	bool		isAttached;
};
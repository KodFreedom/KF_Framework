//--------------------------------------------------------------------------------
//　input.h
//	入力ディバイス情報管理
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class KeyboardDirectX;
class MouseDirectX;
class JoystickDirectX;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum Key
{
	kSubmit = 0,
	kCancel,
	kStart,
	kUp,
	kDown,
	kLeft,
	kRight,
	kReset,
	kLock,
	kKeyMax,

	//重ねるキー
	kJump = kSubmit,
	kAttack = kCancel,
	kSave = kStart,
	kExtend = kRight,
	kShrink = kLeft,
	kRaise = kUp,
	kReduce = kDown,
};

//--------------------------------------------------------------------------------
//  入力情報クラス
//--------------------------------------------------------------------------------
class Input
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : Input*
	//--------------------------------------------------------------------------------
	static Input* Create(HINSTANCE hinstance, HWND hwnd)
	{
		auto instance = MY_NEW Input;
		instance->Init(hinstance, hwnd);
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  アクセス権の取得
	//--------------------------------------------------------------------------------
	void Acquire(void);

	//--------------------------------------------------------------------------------
	//  アクセス権の放棄
	//--------------------------------------------------------------------------------
	void Unacquire(void);

	//--------------------------------------------------------------------------------
	//  移動の水平量の取得
	//--------------------------------------------------------------------------------
	float MoveHorizontal(void) const { return move_horizontal_; }

	//--------------------------------------------------------------------------------
	//  移動の垂直量の取得
	//--------------------------------------------------------------------------------
	float MoveVertical(void) const { return move_vertical_; }

	//--------------------------------------------------------------------------------
	//  回転の水平量の取得
	//--------------------------------------------------------------------------------
	float RotationHorizontal(void) const { return rotation_horizontal_; }

	//--------------------------------------------------------------------------------
	//  回転の垂直量の取得
	//--------------------------------------------------------------------------------
	float RotationVertical(void) const { return rotation_vertical_; }

	//--------------------------------------------------------------------------------
	//  ズーム量の取得
	//--------------------------------------------------------------------------------
	float Zoom(void) const { return zoom_; }

	//--------------------------------------------------------------------------------
	//  プレスの取得
	//--------------------------------------------------------------------------------
	bool GetKeyPress(const Key& key) const { return press_state_ & (1 << static_cast<int>(key)); }

	//--------------------------------------------------------------------------------
	//  トリガーの取得
	//--------------------------------------------------------------------------------
	bool GetKeyTrigger(const Key& key) const { return trigger_state_ & (1 << static_cast<int>(key)); }

	//--------------------------------------------------------------------------------
	//  リリースの取得
	//--------------------------------------------------------------------------------
	bool GetKeyRelease(const Key& key) const { return release_state_ & (1 << static_cast<int>(key)); }

	//--------------------------------------------------------------------------------
	//  エディタモードの設定
	//--------------------------------------------------------------------------------
	void SetEditorMode(const bool& enable) { is_editor_mode_ = enable; }

	//--------------------------------------------------------------------------------
	//  キーボードの取得
	//--------------------------------------------------------------------------------
	const auto GetKeyboard(void) const { return keyboard_; }

	//--------------------------------------------------------------------------------
	//  マウスの取得
	//--------------------------------------------------------------------------------
	const auto GetMouse(void) const { return mouse_; }

	//--------------------------------------------------------------------------------
	//  ジョイスティックの取得
	//--------------------------------------------------------------------------------
	const auto GetJoystick(void) const { return joystick_; }

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	Input();
	Input(const Input& value) {}
	Input& operator=(const Input& value) {}
	~Input() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	bool Init(HINSTANCE hinstance, HWND hwnd);

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  入力情報更新更新
	//--------------------------------------------------------------------------------
	void UpdateInputInfo(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	KeyboardDirectX* keyboard_; // キーボード
	MouseDirectX*    mouse_; // マウス
	JoystickDirectX* joystick_; // ジョイスティック
	float            move_horizontal_; // 移動の水平量
	float            move_vertical_; // 移動の垂直量
	float            rotation_horizontal_; // 回転の水平量
	float            rotation_vertical_; // 回転の垂直量
	float            zoom_; // ズームの量
	LONG             press_state_; // プレスの情報
	LONG             trigger_state_; // トリガーの情報
	LONG             release_state_; // リリースの情報
	bool             is_editor_mode_; // エディタモーションフラグ
};
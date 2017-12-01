//--------------------------------------------------------------------------------
//　camera.h
//	カメラ基底クラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  カメラ基底クラス
//--------------------------------------------------------------------------------
class Camera
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG
	friend class CameraManager;
public:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kDefaultfar_ = 1000.0f;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	Camera();
	~Camera() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual void Init(void);

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	virtual void Update(void) {}

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	virtual void LateUpdate(void)
	{
		UpdateParameter();
	}

	//--------------------------------------------------------------------------------
	//  破棄関数
	//--------------------------------------------------------------------------------
	virtual void Release(void)
	{
		Uninit();
		delete this;
	}

	//--------------------------------------------------------------------------------
	//  セット処理(描画直前)
	//--------------------------------------------------------------------------------
	void Set(void);

	//--------------------------------------------------------------------------------
	//  カメラの範囲内に入ってるかどうか
	//  position : 自分の位置
	//  radius : 判定半径
	//--------------------------------------------------------------------------------
	bool IsInRange(const Vector3& position, const float& radius = kDefaultfar_);
	
	//--------------------------------------------------------------------------------
	//  移動処理
	//  movement : 移動量
	//--------------------------------------------------------------------------------
	void Move(const Vector3& movement)
	{
		rig_.position += movement;
	}

	const auto& GetWorldRight(void) const { return world_right_; }
	const auto& GetWorldUp(void) const { return world_up_; }
	const auto& GetWorldForward(void) const { return world_forward_; }
	const auto& GetWorldEyePosition(void) const { return world_eye_position_; }
	const auto& GetWorldAtPosition(void) const { return world_at_position_; }
	const auto& GetView(void) const { return view_; }
	const auto& GetViewTranspose(void) const { return view_transpose_; }
	const auto& GetProjection(void) const { return projection_; }
	const auto& GetFar(void) const { return far_; }
	virtual void SetDistance(const float& value)
	{
		distance_ = value;
		local_eye_position_.z_ = -distance_;
	}
	void SetWorldPosition(const Vector3& value) { rig_.position = value; }
	void SetOffsetY(const float& value) { pivot_.position.y_ = value; }
	void SetPitch(const float& radian) { pivot_.rotation.x_ = radian; }
	void SetYaw(const float& radian) { rig_.rotation.y_ = radian; }
	void SetFov(const float& radian) { fov_ = radian; }
	void SetNear(const float& value) { near_ = value; }
	void SetFar(const float& value) { far_ = value; }

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct CameraTransform
	{
		Vector3	position;
		Vector3	rotation;
	};

	
	virtual void Uninit(void) {}

	//--------------------------------------------------------------------------------
	//  X軸回転
	//--------------------------------------------------------------------------------
	virtual void Pitch(const float& radian)
	{
		pivot_.rotation.x_ += radian;
	}

	//--------------------------------------------------------------------------------
	//  Y軸回転
	//--------------------------------------------------------------------------------
	virtual void Yaw(const float& radian)
	{
		rig_.rotation.y_ += radian;
	}

	//--------------------------------------------------------------------------------
	//  パラメーターの算出
	//--------------------------------------------------------------------------------
	void UpdateParameter(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CameraTransform rig_;
	CameraTransform pivot_;
	Vector3         local_eye_position_;
	Vector3         world_eye_position_;
	Vector3         world_at_position_;
	Vector3         world_right_;
	Vector3         world_up_;
	Vector3         world_forward_;
	Matrix44        view_;
	Matrix44        view_transpose_;
	Matrix44        projection_;
	float           distance_;
	float           fov_;
	float           near_;
	float           far_;
};
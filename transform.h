//--------------------------------------------------------------------------------
//　transform.h
//  トランスフォームコンポネント
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Transform : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	Transform(GameObject& owner);
	~Transform() {}
	
	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	bool Init(void) override
	{
		UpdateMatrix();
		return true;
	}

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void) override {}

	//--------------------------------------------------------------------------------
	//  行列更新処理（親がない）
	//--------------------------------------------------------------------------------
	void UpdateMatrix(void);

	//--------------------------------------------------------------------------------
	//  行列更新処理（親ある）
	//--------------------------------------------------------------------------------
	void UpdateMatrix(const Matrix44& parent);
	
	//--------------------------------------------------------------------------------
	//  子供登録処理
	//--------------------------------------------------------------------------------
	void RegisterChild(Transform* child);
	
	//--------------------------------------------------------------------------------
	//  子供削除処理
	//--------------------------------------------------------------------------------
	void DeregisterChild(Transform* child);
	
	//--------------------------------------------------------------------------------
	//  親登録処理
	//--------------------------------------------------------------------------------
	void RegisterParent(Transform* value);

	//--------------------------------------------------------------------------------
	//  位置の取得
	//--------------------------------------------------------------------------------
	const Vector3& GetPosition(void) const { return position_; }

    //--------------------------------------------------------------------------------
    //  現在の世界位置の取得
    //--------------------------------------------------------------------------------
    Vector3 GetCurrentWorldPosition(void) const;

	//--------------------------------------------------------------------------------
	//  回転の取得(quaternion)
	//--------------------------------------------------------------------------------
	const Quaternion& GetRotation(void) const { return rotation_; }

	//--------------------------------------------------------------------------------
	//  回転の取得(vector3)
	//--------------------------------------------------------------------------------
	Vector3 GetEulerRotation(void) const
	{
		return rotation_.ToEuler();
	}

	//--------------------------------------------------------------------------------
	//  スケールの取得
	//--------------------------------------------------------------------------------
	const Vector3& GetScale(void) const { return scale_; }
	
	//--------------------------------------------------------------------------------
	//  前方向の取得
	//--------------------------------------------------------------------------------
	Vector3 GetForward(void) const 
	{
		return Vector3::Rotate(Vector3::kForward, rotation_).Normalized();
	}
	
	//--------------------------------------------------------------------------------
	//  上方向の取得
	//--------------------------------------------------------------------------------
	Vector3 GetUp(void) const
	{
		return Vector3::Rotate(Vector3::kUp, rotation_).Normalized();
	}
	
	//--------------------------------------------------------------------------------
	//  右方向の取得
	//--------------------------------------------------------------------------------
	Vector3 GetRight(void) const
	{
		return Vector3::Rotate(Vector3::kRight, rotation_).Normalized();
	}
	
	//--------------------------------------------------------------------------------
	//  回転行列の取得
	//--------------------------------------------------------------------------------
	Matrix44 GetRotationMatrix(void) const
	{
		return rotation_.ToMatrix();
	}

	//--------------------------------------------------------------------------------
	//  最新の世界行列の取得
	//--------------------------------------------------------------------------------
	Matrix44 GetCurrentWorldMatrix(void) const;

	//--------------------------------------------------------------------------------
	//  世界行列の取得
	//--------------------------------------------------------------------------------
	const Matrix44& GetWorldMatrix(void) const { return world_; }

	//--------------------------------------------------------------------------------
	//  親の取得
	//--------------------------------------------------------------------------------
	const auto GetParent(void) const { return parent_; }

	//--------------------------------------------------------------------------------
	//  子供の取得
	//--------------------------------------------------------------------------------
	const auto& GetChildren(void) const { return children_; }

	//--------------------------------------------------------------------------------
	//  位置の設定
	//--------------------------------------------------------------------------------
	void SetPosition(const Vector3& value) { position_ = value; }

	//--------------------------------------------------------------------------------
	//  回転の設定(quaternion)
	//--------------------------------------------------------------------------------
	void SetRotation(const Quaternion& value) { rotation_ = value; }

	//--------------------------------------------------------------------------------
	//  回転の設定(euler)
	//--------------------------------------------------------------------------------
	void SetRotation(const Vector3& euler) { rotation_ = euler.ToQuaternion(); }

	//--------------------------------------------------------------------------------
	//  スケールの設定
	//--------------------------------------------------------------------------------
	void SetScale(const Vector3& value) { scale_ = value; }

	//--------------------------------------------------------------------------------
	//  オーラー角より回転
	//	euler：オーラー角
	//--------------------------------------------------------------------------------
	void RotateByEuler(const Vector3& euler)
	{
		rotation_ *= euler.ToQuaternion();
	}

	//--------------------------------------------------------------------------------
	//  自分のX軸より回転
	//--------------------------------------------------------------------------------
	void RotateByPitch(const float& radian);

	//--------------------------------------------------------------------------------
	//  自分のY軸より回転
	//--------------------------------------------------------------------------------
	void RotateByYaw(const float& radian);

	//--------------------------------------------------------------------------------
	//  自分のZ軸より回転
	//--------------------------------------------------------------------------------
	void RotateByRoll(const float& radian);

	//--------------------------------------------------------------------------------
	//  世界軸の方向ベクトルを自分の軸に変換する
	//	direction：方向ベクトル
	//	return：Vector3
	//--------------------------------------------------------------------------------
	Vector3	TransformDirectionToLocal(const Vector3& direction);

	//--------------------------------------------------------------------------------
	//  子供を探す
	//--------------------------------------------------------------------------------
	Transform* FindChildBy(const String& name);

private:
	//--------------------------------------------------------------------------------
	//  行列の算出(親なし)
	//--------------------------------------------------------------------------------
	void CalculateWorldMatrix(void);

	//--------------------------------------------------------------------------------
	//  行列の算出(親あり)
	//--------------------------------------------------------------------------------
	void CalculateWorldMatrix(const Matrix44& parent);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Vector3                           position_; // 位置
	Quaternion                        rotation_; // 回転
	Vector3                           scale_; // スケール
	Matrix44                          world_; // 世界行列
	Transform*                        parent_; // 親
	unordered_map<String, Transform*> children_; // 子供
};
//--------------------------------------------------------------------------------
//	トランスフォームコンポネント
//　transform.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "transform.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Transform::Transform(GameObject& owner) : Component(owner)
	, position_(Vector3::kZero)
	, scale_(Vector3::kOne)
	, rotation_(Quaternion::kIdentity)
	, world_(Matrix44::kIdentity)
	, parent(nullptr)
	, offset(Matrix44::kIdentity)
{}

//--------------------------------------------------------------------------------
//  マトリクス算出(親がないの場合呼び出される)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(void)
{
	if (parent) return;
	CalculateWorldMatrix();
	for (auto child : children) 
	{
		child->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
//  関数説明：マトリクス算出(親があるの場合呼び出される)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(const Matrix44& parent)
{
	CalculateWorldMatrix(parent);
	for (auto child : children)
	{
		child->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
// 親登録処理
//--------------------------------------------------------------------------------
void Transform::RegisterParent(Transform* value, const Vector3& offset_translation, const Vector3& offset_rotation)
{
	if (parent)
	{//親があるの場合前の親から削除
		parent->DeregisterChild(this);
	}
	parent = value;
	parent->RegisterChild(this);
	offset = Matrix44::Transform(offset_rotation, offset_translation);
}

//--------------------------------------------------------------------------------
//  最新の世界行列の取得
//--------------------------------------------------------------------------------
const Matrix44& Transform::GetCurrentWorldMatrix(void) const
{
	auto& world = Matrix44::Transform(rotation_, position_, scale_);
	if (parent)
	{
		world *= offset;
		world *= parent->GetCurrentWorldMatrix();
	}
	return world;
}

//--------------------------------------------------------------------------------
//  親に対する相対行列の設定
//--------------------------------------------------------------------------------
void Transform::SetOffset(const Vector3& translation, const Vector3& rotation)
{
	if (!parent) return;
	offset = Matrix44::Transform(rotation, translation);
}

//--------------------------------------------------------------------------------
//  自分のX軸より回転
//--------------------------------------------------------------------------------
void Transform::RotateByPitch(const float& radian)
{
	auto& right = GetRight();
	auto& rotation = Quaternion::RotateAxis(right, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//  自分のY軸より回転
//--------------------------------------------------------------------------------
void Transform::RotateByYaw(const float& radian)
{
	auto& up = GetUp();
	auto& rotation = Quaternion::RotateAxis(up, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//  自分のZ軸より回転
//--------------------------------------------------------------------------------
void Transform::RotateByRoll(const float& radian)
{
	auto& forward = GetForward();
	auto& rotation = Quaternion::RotateAxis(forward, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//	世界軸の方向ベクトルを自分の軸に変換する
//--------------------------------------------------------------------------------
Vector3 Transform::TransformDirectionToLocal(const Vector3& direction)
{
	auto& transpose = world_.Transpose();
	return Vector3::TransformNormal(direction, transpose);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	行列の算出(親なし)
//--------------------------------------------------------------------------------
void Transform::CalculateWorldMatrix(void)
{
	world_ = Matrix44::Transform(rotation_, position_, scale_);
}

//--------------------------------------------------------------------------------
//	行列の算出(親あり)
//--------------------------------------------------------------------------------
void Transform::CalculateWorldMatrix(const Matrix44& parent)
{
	CalculateWorldMatrix();
	world_ *= offset;
	world_ *= parent;
}
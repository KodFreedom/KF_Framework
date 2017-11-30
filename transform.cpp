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
#include "game_object.h"

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
	, parent_(nullptr)
	, offset_(Matrix44::kIdentity)
{}

//--------------------------------------------------------------------------------
//  マトリクス算出(親がないの場合呼び出される)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(void)
{
	if (parent_) return;
	CalculateWorldMatrix();
	for (auto& pair : children_) 
	{
		pair.second->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
//  関数説明：マトリクス算出(親があるの場合呼び出される)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(const Matrix44& parent)
{
	CalculateWorldMatrix(parent);
	for (auto& pair : children_)
	{
		pair.second->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
// 親登録処理
//--------------------------------------------------------------------------------
void Transform::RegisterParent(Transform* value, const Vector3& offset_translation, const Quaternion& offset_rotation)
{
	if (parent_)
	{//親があるの場合前の親から削除
		parent_->DeregisterChild(this);
	}
	parent_ = value;
	parent_->RegisterChild(this);
	offset_ = Matrix44::Transform(offset_rotation, offset_translation);
}

//--------------------------------------------------------------------------------
//  最新の世界行列の取得
//--------------------------------------------------------------------------------
const Matrix44& Transform::GetCurrentWorldMatrix(void) const
{
	auto& world = Matrix44::Transform(rotation_, position_, scale_);
	if (parent_)
	{
		world *= offset_;
		world *= parent_->GetCurrentWorldMatrix();
	}
	return world;
}

//--------------------------------------------------------------------------------
//  親に対する相対行列の設定
//--------------------------------------------------------------------------------
void Transform::SetOffset(const Vector3& translation, const Vector3& rotation)
{
	offset_ = Matrix44::Transform(rotation, translation);
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
//  子供を探す
//--------------------------------------------------------------------------------
Transform* Transform::FindChildBy(const String& name)
{
	if (owner_.GetName()._Equal(name)) return this;
	auto iterator = children_.find(name);
	if (children_.end() == iterator)
	{
		for (auto& pair : children_)
		{
			auto result = pair.second->FindChildBy(name);
			if (result) return result;
		}
		return nullptr;
	}
	return iterator->second;
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
	world_ *= offset_;
	world_ *= parent;
}
//--------------------------------------------------------------------------------
//	boxColliderコンポネント
//　boxCollider.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "collider.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class BoxCollider : public Collider
{
public:
	//Get関数
	const auto&	GetLocalVertexes(void) const { return local_vertexes_; }
	auto GetWorldVertexes(void) const
	{
		auto copy = local_vertexes_;
		const auto& world = GetWorldMatrix();
		for (auto& vertex : copy)
		{
			vertex = Vector3::TransformCoord(vertex, world);
		}
		return copy;
	}
	const auto& GetHalfSize(void) const { return half_size_; }

protected:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	BoxCollider(GameObject& owner, const ColliderType& type, const ColliderMode& mode, const Vector3& half_size)
		: Collider(owner, type, mode)
		, half_size_(half_size)
	{
		local_vertexes_.resize(8);
		local_vertexes_[0] = Vector3(-half_size_.x_, -half_size_.y_, half_size_.z_);
		local_vertexes_[1] = Vector3(half_size_.x_, -half_size_.y_, half_size_.z_);
		local_vertexes_[2] = Vector3(-half_size_.x_, -half_size_.y_, -half_size_.z_);
		local_vertexes_[3] = Vector3(half_size_.x_, -half_size_.y_, -half_size_.z_);
		local_vertexes_[4] = Vector3(-half_size_.x_, half_size_.y_, half_size_.z_);
		local_vertexes_[5] = Vector3(half_size_.x_, half_size_.y_, half_size_.z_);
		local_vertexes_[6] = Vector3(-half_size_.x_, half_size_.y_, -half_size_.z_);
		local_vertexes_[7] = Vector3(half_size_.x_, half_size_.y_, -half_size_.z_);
	}
	~BoxCollider() {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Vector3         half_size_;
	vector<Vector3> local_vertexes_;
};
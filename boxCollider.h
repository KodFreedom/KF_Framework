//--------------------------------------------------------------------------------
//	boxColliderコンポネント
//　boxCollider.h
//	Author : Xu Wenjie
//	Date   : 2017-08-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "collider.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class BoxCollider : public Collider
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	BoxCollider(GameObject* const owner, const ColliderType& type, const ColliderMode& mode, const Vector3& halfSize)
		: Collider(owner, type, mode)
		, halfSize(halfSize)
	{
		Vector3 vertexes[8] = {
			{ -halfSize.X, -halfSize.Y, halfSize.Z },
			{ halfSize.X, -halfSize.Y, halfSize.Z },
			{ -halfSize.X, -halfSize.Y, -halfSize.Z },
			{ halfSize.X, -halfSize.Y, -halfSize.Z },
			{ -halfSize.X, halfSize.Y, halfSize.Z },
			{ halfSize.X, halfSize.Y, halfSize.Z },
			{ -halfSize.X, halfSize.Y, -halfSize.Z },
			{ halfSize.X, halfSize.Y, -halfSize.Z } };
		for (auto& vertex : vertexes) localVertexes.push_back(vertex);
	}
	~BoxCollider() {}

	//Get関数
	auto			GetLocalVertexes(void) const { return localVertexes; }
	list<Vector3>	GetNextWorldVertexes(void) const;
	auto			GetHalfSize(void) const { return halfSize; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	Vector3			halfSize;
	list<Vector3>	localVertexes;
};
//--------------------------------------------------------------------------------
//	boxCollider�R���|�l���g
//�@boxCollider.h
//	Author : Xu Wenjie
//	Date   : 2017-08-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "collider.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class BoxCollider : public Collider
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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

	//Get�֐�
	auto			GetLocalVertexes(void) const { return localVertexes; }
	list<Vector3>	GetNextWorldVertexes(void) const;
	auto			GetHalfSize(void) const { return halfSize; }

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	Vector3			halfSize;
	list<Vector3>	localVertexes;
};
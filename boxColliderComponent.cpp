//--------------------------------------------------------------------------------
//	boxCollider�R���|�l���g
//�@boxColliderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-28
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "boxColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
list<CKFVec3> CBoxColliderComponent::GetWorldVertexes(void)
{
	auto& listVtx = GetLocalVertexes();
	const auto& mtxWorld = GetMatrixWorld();
	for (auto itr = listVtx.begin(); itr != listVtx.end(); ++itr)
	{
		*itr = CKFMath::Vec3TransformCoord(*itr, mtxWorld);
	}
	return listVtx;
}
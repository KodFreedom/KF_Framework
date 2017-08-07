//--------------------------------------------------------------------------------
//	AABBCollider�R���|�l���g
//�@AABBColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-28
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "boxColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  AABBCollider�|�l���g�N���X
//--------------------------------------------------------------------------------
class CAABBColliderComponent : public CBoxColliderComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CAABBColliderComponent(CGameObject* const pGameObj, const CS::COL_MODE& mode, const CKFVec3& vHalfSize)
		: CBoxColliderComponent(pGameObj, CS::COL_AABB, mode, vHalfSize) {}
	~CAABBColliderComponent() {}

	void	Update(void) override
	{
		CBoxColliderComponent::Update();

		//��]������������
		m_mtxWorld.m_af[0][0] = 1.0f;
		m_mtxWorld.m_af[0][1] = 0.0f;
		m_mtxWorld.m_af[0][2] = 0.0f;
		m_mtxWorld.m_af[1][0] = 0.0f;
		m_mtxWorld.m_af[1][1] = 1.0f;
		m_mtxWorld.m_af[1][2] = 0.0f;
		m_mtxWorld.m_af[2][0] = 0.0f;
		m_mtxWorld.m_af[2][1] = 0.0f;
		m_mtxWorld.m_af[2][2] = 1.0f;
	}

private:
};
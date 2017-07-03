//--------------------------------------------------------------------------------
//	�R���W�����R���|�l���g
//�@collisionComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "colliderComponent.h"
#include "manager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CColliderComponent::CColliderComponent(CGameObject* const pGameObj, const CM::COL_TYPE& type, const CM::MODE& mode, const CKFVec3& vPos) : CComponent(pGameObj)
	, m_type(type)
	, m_mode(mode)
	, m_bTrigger(false)
	, m_vPos(vPos)
{
	if (m_type < CM::COL_MAX)
	{
		m_itr = GetManager()->GetColliderManager()->SaveCollider(mode, type, this);
	}
	else
	{
		switch (m_type)
		{
		case CM::COL_FIELD:
			m_itr = GetManager()->GetColliderManager()->SaveField(this);
			break;
		default:
			break;
		}
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CColliderComponent::Uninit(void)
{
	if (m_type < CM::COL_MAX)
	{
		GetManager()->GetColliderManager()->ReleaseCollider(m_mode, m_type, m_itr);
	}
	else
	{
		switch (m_type)
		{
		case CM::COL_FIELD:
			GetManager()->GetColliderManager()->ReleaseField(m_itr);
			break;
		default:
			break;
		}
	}
}

//--------------------------------------------------------------------------------
//  ���[���h���W�擾
//--------------------------------------------------------------------------------
const CKFVec3 CColliderComponent::GetWorldPos(void) const
{
	CKFMtx44 mtx = GetGameObject()->GetMatrixNext();
	CKFVec3 vPos = m_vPos;
	CKFMath::Vec3TransformCoord(&vPos, mtx);
	return vPos;
}
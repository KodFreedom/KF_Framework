//--------------------------------------------------------------------------------
//	collider�}�l�[�W��
//�@colliderManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "colliderManager.h"
#include "colliderComponent.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CColliderManager::CColliderManager()
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CColliderManager::Init(void)
{

}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CColliderManager::Uninit(void)
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CColliderManager::Update(void)
{
	//Dynamic�w�̃R���W�������Ƃɂ܂�Static�w�Ɠ����蔻�肵��
	//Dynamic�w�̑��̃R���W�����Ɠ����蔻�肷��
	//Sphere
	if (!m_alistCollider[DYNAMIC][COL_SPHERE].empty())
	{
		for (COL_ITR itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
		{
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
		}
	}
	
}

////--------------------------------------------------------------------------------
////  Dynamic�̃R���W������Static�̃R���W�����Ɠ����蔻��
////--------------------------------------------------------------------------------
//void CColliderManager::CheckInStatic(const std::list<CColliderComponent*>::iterator& itrThis)
//{
//	
//	if (!m_alistCollision[STATIC].empty())
//	{
//		std::list<CColliderComponent*>::iterator itrTarget;
//
//		for (itrTarget = m_alistCollision[STATIC].begin(); itrTarget != m_alistCollision[STATIC].end(); itrTarget++)
//		{
//			CheckCollision(itrThis, itrTarget);
//		}
//	}
//}
//
////--------------------------------------------------------------------------------
////  Dynamic�̃R���W������Static�̃R���W�����Ɠ����蔻��
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const std::list<CColliderComponent*>::iterator& itrL, const std::list<CColliderComponent*>::iterator& itrR)
//{
//	switch ((*itrL)->GetType())
//	{
//	case CColliderComponent::COL_SPHERE:
//		CSphereColliderComponent* pL = (CSphereColliderComponent*)&(*itrL);
//		break;
//	case CColliderComponent::COL_FIELD:
//		CFieldCollisionComponent* pL = (CFieldCollisionComponent*)&(*itrL);
//	default:
//		break;
//	}
//
//	
//}
//
////--------------------------------------------------------------------------------
////  �X�t�B�A���t�B�[���h�Ɠ����蔻��
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const CSphereColliderComponent& sphere, const CFieldCollisionComponent& field)
//{
//
//}
//
////--------------------------------------------------------------------------------
////  �X�t�B�A���t�B�[���h�Ɠ����蔻��
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const CFieldCollisionComponent& field, const CSphereColliderComponent& sphere)
//{
//	CheckCollision(sphere, field);
//}

//--------------------------------------------------------------------------------
//  Save����
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return; }
	COL_ITR itr;
	m_alistCollider[mode][type].push_back(pCollider);
	itr = m_alistCollider[mode][type].begin();
	return itr;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void CColliderManager::ReleaseCollider(const MODE& mode, const COL_TYPE& type, const COL_ITR& itr)
{
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return; }
	m_alistCollider[mode][type].erase(itr);
}
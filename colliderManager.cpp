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
#include "fieldColliderComponent.h"
#include "KF_Collision.h"

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

	m_listField.clear();
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

	m_listField.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CColliderManager::Update(void)
{
	//Dynamic�w�̃R���W�������Ƃɂ܂�FStatic�w�Ɠ����蔻�肵��
	//Dynamic�w�̑��̃R���W�����Ɠ����蔻�肵��
	//�Ō��Field�Ɠ����蔻�肷��
	//Sphere
	if (!m_alistCollider[DYNAMIC][COL_SPHERE].empty())
	{
		for (COL_ITR itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
		{
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);

			//Dynamic

			//Static

			//Field
			CheckWithField(*pSphere);
		}
	}
}


//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CColliderManager::CheckWithField(CSphereColliderComponent& sphere)
{	
	if (!m_listField.empty())
	{
		for (COL_ITR itrField = m_listField.begin(); itrField != m_listField.end(); itrField++)
		{
			CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itrField);
			CKFCollision::CheckCollisionSphereWithField(sphere, *pField);
		}
	}
}

//--------------------------------------------------------------------------------
//  Save����
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	COL_ITR itr;
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return itr; }
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

//--------------------------------------------------------------------------------
//  Save����(Field)
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveField(CColliderComponent* pCollider)
{
	COL_ITR itr;
	if (pCollider->GetType() != COL_FIELD) { return itr; }
	m_listField.push_front(pCollider);
	itr = m_listField.begin();
	return itr;
}

//--------------------------------------------------------------------------------
//  �j������(Field)
//--------------------------------------------------------------------------------
void CColliderManager::ReleaseField(const COL_ITR& itr)
{
	m_listField.erase(itr);
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�����蔻��
//--------------------------------------------------------------------------------
bool CColliderManager::SphereCast(const CKFVec3& vCenter, const float& fRadius, HIT_INFO& hitInfoOut)
{
	if (!m_listField.empty())
	{
		for (COL_ITR itrField = m_listField.begin(); itrField != m_listField.end(); itrField++)
		{
			CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itrField);
			CFieldColliderComponent::INFO info = pField->GetPointInfo(vCenter);
			if (!info.bInTheField) { continue; }
			if (info.fHeight > vCenter.m_fY - fRadius)
			{
				hitInfoOut.vNormal = info.vFaceNormal;
				hitInfoOut.vPos = CKFVec3(vCenter.m_fX, info.fHeight, vCenter.m_fZ);
				return true;
			}
		}
	}
	return false;
}
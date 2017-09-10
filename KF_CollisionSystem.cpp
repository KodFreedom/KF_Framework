//--------------------------------------------------------------------------------
//	�R���W��������N���X
//�@KF_Collision.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "KF_CollisionSystem.h"
#include "KF_CollisionUtility.h"
#include "KF_PhysicsSystem.h"
#include "gameObject.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "boxColliderComponent.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
#include "rendererDX.h"
#include "textureManager.h"
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CKFCollisionSystem::CKFCollisionSystem()
#ifdef _DEBUG
	: m_pMeshSphere(nullptr)
	, m_pMeshCube(nullptr)
#endif
{
	for (auto& aList : m_alistCollider)
	{
		for (auto& list : aList)
		{
			list.clear();
		}
	}

	m_listField.clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Init(void)
{
#ifdef _DEBUG
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	CMain::GetManager()->GetTextureManager()->UseTexture("polygon.png");
	D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_pMeshSphere, nullptr);
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_pMeshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Uninit(void)
{
	for (auto& aList : m_alistCollider)
	{
		for (auto& list : aList)
		{
			list.clear();
		}
	}

	m_listField.clear();

#ifdef _DEBUG
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshCube);
	CMain::GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Update(void)
{
	//Dynamic�w�̃R���W�������Ƃɂ܂�FStatic�w�Ɠ����蔻�肵��
	//Dynamic�w�̑��̃R���W�����Ɠ����蔻�肵��
	//�Ō��Field�Ɠ����蔻�肷��
	//Sphere
	for (auto itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); ++itr)
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicSphere(itr, *pSphere); 
#pragma omp section 
			checkWithDynamicAABB(*pSphere); 
#pragma omp section 
			checkWithDynamicOBB(*pSphere);
#pragma omp section 
			checkWithStaticSphere(*pSphere);
#pragma omp section 
			checkWithStaticAABB(*pSphere);
#pragma omp section
			checkWithStaticOBB(*pSphere);
#pragma omp section
			checkWithField(*pSphere);
		}
	}

	//aabb
	for (auto itr = m_alistCollider[DYNAMIC][COL_AABB].begin(); itr != m_alistCollider[DYNAMIC][COL_AABB].end(); ++itr)
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicAABB(itr, *pAABB);
#pragma omp section 
			checkWithDynamicOBB(*pAABB);
#pragma omp section 
			checkWithStaticSphere(*pAABB);
#pragma omp section 
			checkWithStaticAABB(*pAABB);
#pragma omp section 
			checkWithStaticOBB(*pAABB);
#pragma omp section
			checkWithField(*pAABB);
		}
	}

	//obb
	for (auto itr = m_alistCollider[DYNAMIC][COL_OBB].begin(); itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicOBB(itr, *pOBB);
#pragma omp section 
			checkWithStaticSphere(*pOBB);
#pragma omp section 
			checkWithStaticAABB(*pOBB);
#pragma omp section 
			checkWithStaticOBB(*pOBB);
#pragma omp section 
			checkWithField(*pOBB);
		}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::LateUpdate(void)
{
#ifdef _DEBUG
	auto pDebug = CMain::GetManager()->GetDebugManager();
	pDebug->DisplayAlways("Collider��� : \n");
	for (int nCntMode = 0; nCntMode < MODE_MAX; ++nCntMode)
	{
		pDebug->DisplayAlways(toString((COL_MODE)nCntMode) + " : ");
		for (int nCntType = 0; nCntType < COL_MAX; ++nCntType)
		{
			int nNumCol = (int)m_alistCollider[nCntMode][nCntType].size();
			pDebug->DisplayAlways(
				toString((COL_TYPE)nCntType) +
				"-" +
				to_string(nNumCol) + "    ");
		}
		pDebug->DisplayAlways('\n');
	}
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].push_back(pCollider);
		return;
	}
	switch (type)
	{
	case CS::COL_FIELD:
		m_listField.push_back(pCollider);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DeregisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].remove(pCollider);
		return;
	}

	switch (type)
	{
	case CS::COL_FIELD:
	{
		m_listField.remove(pCollider);
		break;
	}
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//	�֐����FRayCast
//  �֐������F���C�L���X�g�֐��A���C�ƏՓ˂����̍ŋߓ_���Z�o����
//	�����F	vOrigin�F���C�̎n�_
//			vDirection�F���C�̌���
//			fDistance�F���C�̒���
//			infoOut�F�Փ˓_�̏��
//			pObjThis�F�����̃Q�[���I�u�W�F�N�g
//	�߂�l�F�Փ˃t���O
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::RayCast(const CKFVec3& vOrigin, const CKFVec3& vDirection, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	CKFRay ray = CKFRay(vOrigin, vDirection);
	bool bFind = false;
	CRaycastHitInfo info;
	
#pragma omp parallel sections
	{
#pragma omp section
		{
			if (checkWithDynamicAABB(ray, fDistance, info, pObjThis))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithDynamicOBB(ray, fDistance, info, pObjThis))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithStaticAABB(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithStaticOBB(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithField(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}
	}

	return bFind;
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DrawCollider(void)
{
	//LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	////sphere
	//LPDIRECT3DTEXTURE9 pTexture = CMain::GetManager()->GetTextureManager()->GetTexture("polygon.png");
	//for (auto itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
	//{
	//	D3DXVECTOR3 vPos = (*itr)->GetWorldPos();
	//	float fRadius = ((CSphereColliderComponent*)(*itr))->GetRadius();
	//	D3DXMATRIX mtx,mtxPos,mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, vPos.x, vPos.y, vPos.z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	pDevice->SetTexture(0, pTexture);
	//	m_pMeshSphere->DrawSubset(0);
	//}

	////Cube
	//for (auto pCol : m_alistCollider[STATIC][COL_AABB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) { continue; }
	//	auto vPos = pCol->GetWorldPos();
	//	auto vHalfSize = ((CAABBColliderComponent*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.m_fX * 2.0f, vHalfSize.m_fY * 2.0f, vHalfSize.m_fZ * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, vPos.m_fX, vPos.m_fY, vPos.m_fZ);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	pDevice->SetTexture(0, pTexture);
	//	m_pMeshCube->DrawSubset(0);
	//}
}
#endif

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  �Փˏ���
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); ++itr)
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);
		CCollisionDetector::CheckSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicAABB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(sphere, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//  AABB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicAABB(const list<CColliderComponent*>::iterator& itrBegin, CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == aabb.GetGameObject()) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckAABBWithAABB(aabb, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FcheckWithDynamicOBB
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithDynamicAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == pObjThis) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pAABB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}


//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == aabb.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(aabb, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  OBB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(const list<CColliderComponent*>::iterator& itrBegin, COBBColliderComponent& obb)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if ((*itr)->GetGameObject() == obb.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		CCollisionDetector::CheckBoxWithBox(*pOBB, obb);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FcheckWithDynamicOBB
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithDynamicOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		CRaycastHitInfo info;
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if (pCollider->GetGameObject() == pObjThis) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pOBB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AAB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(*pSphere, aabb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(COBBColliderComponent& obb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(*pSphere, obb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(sphere, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckAABBWithAABB(*pAABB, aabb);
	}
}

//--------------------------------------------------------------------------------
//  OBB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(COBBColliderComponent& obb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(*pAABB, obb);
	}
}

//--------------------------------------------------------------------------------
//  OBB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithStaticAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pAABB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  box��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(CBoxColliderComponent& box)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(*pOBB, box);
	}
}

//--------------------------------------------------------------------------------
//  box��OBB�̓����蔻��
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithStaticOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		CRaycastHitInfo info;
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pOBB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithField(sphere, *pField);
	}
}

//--------------------------------------------------------------------------------
//  OBB�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(CBoxColliderComponent& box)
{
	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithField(box, *pField);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FRayCast
//  �֐������F���C�L���X�g�֐��A���C�ƏՓ˂����̍ŋߓ_���Z�o����
//	�����F	vOrigin�F���C�̎n�_
//			vDirection�F���C�̌���
//			fDistance�F���C�̒���
//			infoOut�F�Փ˓_�̏��
//			pObjThis�F�����̃Q�[���I�u�W�F�N�g
//	�߂�l�F�Փ˃t���O
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithField(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;

	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithField(ray, fDistance, *pField, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if(info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//	�֐����FtoString
//  �֐������F���[�h��enum��string�ɕϊ�
//	�����F	mode
//	�߂�l�Fstring
//--------------------------------------------------------------------------------
string CKFCollisionSystem::toString(const COL_MODE& mode)
{
	switch (mode)
	{
	case STATIC:
		return "Static";
	case DYNAMIC:
		return "Dynamic";
	default:
		return "Error";
	}
}

//--------------------------------------------------------------------------------
//	�֐����FtoString
//  �֐������F�^�C�v��enum��string�ɕϊ�
//	�����F	type
//	�߂�l�Fstring
//--------------------------------------------------------------------------------
string CKFCollisionSystem::toString(const COL_TYPE& type)
{
	switch (type)
	{
	case COL_SPHERE:
		return "Sphere";
	case COL_AABB:
		return "AABB";
	case COL_OBB:
		return "OBB";
	case COL_PLANE:
		return "Plane";
	case COL_CYLINDER:
		return "Cylinder";
	default:
		return "Error";
	}
}
#endif
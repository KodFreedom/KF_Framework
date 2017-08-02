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
#include "KF_PhysicsSystem.h"
#include "gameObject.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "OBBColliderComponent.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#ifdef _DEBUG
#include "rendererDX.h"
#include "textureManager.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
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
void CKFCollisionSystem::Init(void)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	GetManager()->GetTextureManager()->UseTexture("polygon.png");
	D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_pMeshSphere, nullptr);
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_pMeshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Uninit(void)
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}

	m_listField.clear();

#ifdef _DEBUG
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshCube);
	GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
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
		CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);

		//Dynamic
		CheckWithDynamicSphere(itr, *pSphere);

		//Static
		CheckWithStaticSphere(*pSphere);

		//Field
		CheckWithField(*pSphere);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::LateUpdate(void)
{
#ifndef _DEBUG
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}

	m_listField.clear();
#endif // !_DEBUG
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CKFCollisionSystem::RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].push_back(pCollider);
	}
	else
	{
		switch (type)
		{
		case CS::COL_FIELD:
			m_listField.push_back(pCollider);
			break;
		default:
			break;
		}
	}
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DrawCollider(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	////sphere
	//LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture("polygon.png");
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

	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}

	m_listField.clear();
}
#endif

//--------------------------------------------------------------------------------
//  �Փˏ���
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); ++itr)
	{
		//�����I�u�W�F�N�g�ɕt���Ă���Ȃ画�肵�Ȃ�
		if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
		CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
		CheckCollisionSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckWithStaticSphere(CSphereColliderComponent& sphere)
{
	for (auto itr = m_alistCollider[STATIC][COL_SPHERE].begin(); itr != m_alistCollider[STATIC][COL_SPHERE].end(); ++itr)
	{
		CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
		CheckCollisionSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckWithField(CSphereColliderComponent& sphere)
{
	for (auto itr = m_listField.begin(); itr != m_listField.end(); ++itr)
	{
		CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itr);
		CheckCollisionSphereWithField(sphere, *pField);
	}
}

//--------------------------------------------------------------------------------
//  �Փ˔���֐�
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR)
{
	CKFVec3 vSLPos = sphereL.GetWorldPos();
	float fSLRadius = sphereL.GetRadius();
	CKFVec3 vSRPos = sphereR.GetWorldPos();
	float fSRRadius = sphereR.GetRadius();
	CKFVec3 vMidLine = vSLPos - vSRPos;
	float fDisMin = fSLRadius + fSRRadius;
	float fDisSqr = CKFMath::VecMagnitudeSquare(vMidLine);

	if (fDisSqr >= fDisMin * fDisMin) { return; }

	if (sphereL.IsTrigger() || sphereR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto list = sphereL.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphereL, sphereR);
		}

		list = sphereR.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphereR, sphereL);
		}
		return;
	}

	CCollision collision;
		
	//�Փ˓_�̎Z�o
	collision.m_vCollisionPos = vSRPos + vMidLine * 0.5f;

	//�Փː[�x�̎Z�o
	float fDis = sqrtf(fDisSqr);
	collision.m_fPenetration = fDisMin - fDis;

	//�Փ˖@���̎Z�o
	collision.m_vCollisionNormal = vMidLine / fDis;

	//���W�b�h�{�f�B�̎擾
	CRigidbodyComponent* pRBL = sphereL.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision.m_vCollisionNormal *= -1.0f;
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo info;
	info.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	info.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	info.m_listCollision.push_back(&collision);
	auto list = sphereL.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphereL, info);
	}

	list = sphereR.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphereR, info);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb)
{
	CKFVec3 vSPos = sphere.GetWorldPos();
	float fSRadius = sphere.GetRadius();
	CKFVec3 vBHalfSize = obb.GetHalfSize();
	CKFMtx44 mtxBox = obb.GetMatrixWorld();
	CKFVec3 vRealSPos = CKFMath::TransformInverse(mtxBox, vSPos);
	
	//�������`�F�b�N
	if (fabsf(vRealSPos.m_fX) - fSRadius > vBHalfSize.m_fX
		|| fabsf(vRealSPos.m_fY) - fSRadius > vBHalfSize.m_fY
		|| fabsf(vRealSPos.m_fZ) - fSRadius > vBHalfSize.m_fZ)
	{
		return;
	}

	CKFVec3 vClosestPos;
	float fDis;

	//OBB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	fDis = vRealSPos.m_fX;
	if (fDis > vBHalfSize.m_fX) { fDis = vBHalfSize.m_fX; }
	if (fDis < -vBHalfSize.m_fX) { fDis = -vBHalfSize.m_fX; }
	vClosestPos.m_fX = fDis;

	fDis = vRealSPos.m_fY;
	if (fDis > vBHalfSize.m_fY) { fDis = vBHalfSize.m_fY; }
	if (fDis < -vBHalfSize.m_fY) { fDis = -vBHalfSize.m_fY; }
	vClosestPos.m_fY = fDis;

	fDis = vRealSPos.m_fZ;
	if (fDis > vBHalfSize.m_fZ) { fDis = vBHalfSize.m_fZ; }
	if (fDis < -vBHalfSize.m_fZ) { fDis = -vBHalfSize.m_fZ; }
	vClosestPos.m_fZ = fDis;

	//�Փˌ��m
	float fDisSqr = CKFMath::VecMagnitudeSquare(vClosestPos - vRealSPos);
	if (fDisSqr >= fSRadius * fSRadius) { return; }

	//�Փˏ��
	CKFMath::MtxTranslation(mtxBox, vClosestPos);
	CCollision collision;
	collision.m_vCollisionNormal = vClosestPos - vSPos;
	CKFMath::VecNormalize(collision.m_vCollisionNormal);
	collision.m_vCollisionPos = vClosestPos;
	collision.m_fPenetration = fSRadius - sqrtf(fDisSqr);
	
	//���W�b�h�{�f�B�̎擾
	CRigidbodyComponent* pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = obb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision.m_vCollisionNormal *= -1.0f;
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo info;
	info.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	info.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	info.m_listCollision.push_back(&collision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphere, info);
	}

	list = obb.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(obb, info);
	}
}

//--------------------------------------------------------------------------------
//  field
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	CKFVec3 vSpherePos = sphere.GetWorldPos();
	float fSphereRadius = sphere.GetRadius();
	CFieldColliderComponent::INFO info = field.GetPointInfo(vSpherePos);

	if (info.bInTheField == false) { return; }

	//�X�t�B�A���S�ƃ|���S���̋����̎Z�o
	float fDis = vSpherePos.m_fY - fSphereRadius - info.fHeight;

	if (fDis >= 0.0f) { return; }

	if (sphere.IsTrigger())
	{
		auto list = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphere, field);
		}
		return;
	}
	CCollision collision;

	//�Փ˓_�̎Z�o
	collision.m_vCollisionPos = CKFVec3(vSpherePos.m_fX, info.fHeight, vSpherePos.m_fZ);

	//�Փː[�x�̎Z�o
	collision.m_fPenetration = -fDis;

	//�Փ˖@���̎Z�o
	collision.m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f);

	//���W�b�h�{�f�B�̎擾
	C3DRigidbodyComponent* p3DRB = dynamic_cast<C3DRigidbodyComponent*>(sphere.GetGameObject()->GetRigidbodyComponent());
	p3DRB->SetOnGround(true);
	collision.m_pRigidBodyOne = p3DRB;
	collision.m_pRigidBodyTwo = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	cInfo.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	cInfo.m_listCollision.push_back(&collision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphere, cInfo);
	}

	list = field.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(field, cInfo);
	}

	//CGameObject* pSphereObj = sphere.GetGameObject();

	////�ʒu����
	//CKFVec3 vMovement = CKFVec3(0.0f, info.fHeight + fSphereRadius - vSpherePos.m_fY, 0.0f);
	//pSphereObj->GetTransformComponent()->MovePosNext(vMovement);

	////��]����
	////CKFVec3 vUpNext = CKFMath::LerpNormal(pSphereObj->GetUpNext(), info.vFaceNormal, 0.2f);
	////pSphereObj->RotByUp(vUpNext);

	////���x����
	//CRigidbodyComponent* pRB = sphere.GetGameObject()->GetRigidbodyComponent();
	//if (pRB->GetType() == CRigidbodyComponent::RB_3D)
	//{
	//	C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;
	//	p3DRB->SetOnGround(true);
	//	CKFVec3 vVelocity = p3DRB->GetVelocity();
	//	vVelocity.m_fY = 0.0f;
	//	p3DRB->SetVelocity(vVelocity);
	//}
}

//--------------------------------------------------------------------------------
//  OBB�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionOBBWithField(COBBColliderComponent& obb, CFieldColliderComponent& field)
{

}

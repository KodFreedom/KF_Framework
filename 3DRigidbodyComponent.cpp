//--------------------------------------------------------------------------------
//
//�@3DRigidbodyComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "KF_PhysicsSystem.h"
#include "3DRigidbodyComponent.h"
#include "colliderComponent.h"
#include "sphereColliderComponent.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
C3DRigidbodyComponent::C3DRigidbodyComponent(CGameObject* const pGameObj)
	: CRigidbodyComponent(pGameObj, RB_3D)
	, m_fMass(1.0f)
	, m_fInverseMass(1.0f)
	, m_fDrag(1.0f)
	//, m_fAngularDrag(0.95f)
	, m_fFriction(1.0f)
	, m_fBounciness(0.0f)
	, m_fGravityCoefficient(5.0f)
	, m_vMovement(CKFMath::sc_vZero)
	, m_vVelocity(CKFMath::sc_vZero)
	, m_vAcceleration(CKFMath::sc_vZero)
	, m_vForceAccum(CKFMath::sc_vZero)
	//, m_bRotLock(0)
{
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::SetMass(const float& fMass)
{
	if (fMass <= 0.0f) { return; }
	m_fMass = fMass;
	m_fInverseMass = 1.0f / fMass;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::Update(void)
{
	auto pTrans = m_pGameObj->GetTransformComponent();

	//�d�͉����x
	m_vAcceleration += CKFPhysicsSystem::sc_vGravity * m_fGravityCoefficient;

	//�͂�������x���v�Z����
	m_vAcceleration += m_vForceAccum * m_fInverseMass;
	
	//��]�͂����]�����x���v�Z����
	//CKFMtx44 mtxIitWorld;
	//calculateInertiaTensorWorld(mtxIitWorld);
	//CKFVec3 vAngularAcceleration = CKFMath::Vec3TransformCoord(m_vTorqueAccum, mtxIitWorld);

	//���x
	float fD = DELTA_TIME;
	m_vVelocity += m_vAcceleration * DELTA_TIME;
	//m_vAngularVelocity   += vAngularAcceleration;

	//�ʒu�X�V
	pTrans->MovePosNext(m_vVelocity * DELTA_TIME);
	pTrans->MovePosNext(m_vMovement);

	//��]�X�V
	//pTrans->RotByEuler(m_vAngularVelocity);

	//��������
	m_vForceAccum = CKFMath::sc_vZero;
	m_vAcceleration = CKFMath::sc_vZero;
	//m_vTorqueAccum = CKFVec3(0.0f);
	m_vVelocity *= m_fDrag;
	//m_vAngularVelocity *= m_fAngularDrag;
	m_vMovement = CKFMath::sc_vZero;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::LateUpdate(void)
{
	//�ʒu�X�V
	m_pGameObj->GetTransformComponent()->MovePosNext(m_vMovement);

	//��������
	m_vMovement = CKFMath::sc_vZero;
}

//--------------------------------------------------------------------------------
//  �����e���\���̎Z�o
//--------------------------------------------------------------------------------
//void C3DRigidbodyComponent::SetInertiaTensor(CColliderComponent* pCollider)
//{
//	float fV00 = 1.0f;
//	float fV11 = 1.0f;
//	float fV22 = 1.0f;
//	CS::COL_TYPE type = pCollider->GetType();
//	if (type == CS::COL_SPHERE)
//	{
//		float fRadius = dynamic_cast<CSphereColliderComponent*>(pCollider)->GetRadius();
//		fV00 
//			= fV11
//			= fV22
//			= 0.4f * m_fMass * fRadius * fRadius;
//	}
//	else if (type == CS::COL_AABB)
//	{
//		CKFVec3 vSize = dynamic_cast<CAABBColliderComponent*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = m_fMass / 12.0f;
//		fV00 = fWork * (vSize.m_fY * vSize.m_fY + vSize.m_fZ * vSize.m_fZ);
//		fV11 = fWork * (vSize.m_fX * vSize.m_fX + vSize.m_fZ * vSize.m_fZ);
//		fV22 = fWork * (vSize.m_fX * vSize.m_fX + vSize.m_fY * vSize.m_fY);
//	}
//	else if (type == CS::COL_OBB)
//	{
//		CKFVec3 vSize = dynamic_cast<COBBColliderComponent*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = m_fMass / 12.0f;
//		fV00 = fWork * (vSize.m_fY * vSize.m_fY + vSize.m_fZ * vSize.m_fZ);
//		fV11 = fWork * (vSize.m_fX * vSize.m_fX + vSize.m_fZ * vSize.m_fZ);
//		fV22 = fWork * (vSize.m_fX * vSize.m_fX + vSize.m_fY * vSize.m_fY);
//	}
//
//	m_mtxInertisTensor = {
//		fV00, 0.0f, 0.0f, 0.0f,
//		0.0f, fV11, 0.0f, 0.0f,
//		0.0f, 0.0f, fV22, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f };
//}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���[���h�����e���\���̎Z�o
//--------------------------------------------------------------------------------
//void C3DRigidbodyComponent::calculateInertiaTensorWorld(CKFMtx44& mtxIitWorld)
//{
//	auto listCollider = m_pGameObj->GetColliderComponent();
//	if (!listCollider.empty())
//	{
//		//���[���h�}�g���N�X�̎擾
//		auto itrBegin = listCollider.begin();
//		CKFMtx44 mtxWorld = (*itrBegin)->GetMatrixWorld();
//		
//		//�ړ��ʂ𖳂���
//		mtxWorld.m_af[3][0] = 0.0f;
//		mtxWorld.m_af[3][1] = 0.0f;
//		mtxWorld.m_af[3][2] = 0.0f;
//
//		//���[���h�����e���\���̎Z�o
//		mtxIitWorld = m_mtxInertisTensor * mtxWorld;
//		
//		//�t�s��̎Z�o
//		mtxIitWorld = CKFMath::MtxTranspose(mtxIitWorld);
//	}
//
//	/*auto listCollider = m_pGameObj->GetColliderComponent();
//	if (!listCollider.empty())
//	{
//		auto itrBegin = listCollider.begin();
//		const CKFMtx44& mtxWorld = (*itrBegin)->GetMatrixWorld();
//		float fT4 = mtxWorld.m_af[0][0] * m_mtxInverseInertisTensor.m_af[0][0]
//			+ mtxWorld.m_af[1][0] * m_mtxInverseInertisTensor.m_af[0][1]
//			+ mtxWorld.m_af[2][0] * m_mtxInverseInertisTensor.m_af[0][2];
//		float fT9 = mtxWorld.m_af[0][0] * m_mtxInverseInertisTensor.m_af[1][0]
//			+ mtxWorld.m_af[1][0] * m_mtxInverseInertisTensor.m_af[1][1]
//			+ mtxWorld.m_af[2][0] * m_mtxInverseInertisTensor.m_af[1][2];
//		float fT14 = mtxWorld.m_af[0][0] * m_mtxInverseInertisTensor.m_af[2][0]
//			+ mtxWorld.m_af[1][0] * m_mtxInverseInertisTensor.m_af[2][1]
//			+ mtxWorld.m_af[2][0] * m_mtxInverseInertisTensor.m_af[2][2];
//		float fT28 = mtxWorld.m_af[0][1] * m_mtxInverseInertisTensor.m_af[0][0]
//			+ mtxWorld.m_af[1][1] * m_mtxInverseInertisTensor.m_af[0][1]
//			+ mtxWorld.m_af[2][1] * m_mtxInverseInertisTensor.m_af[0][2];
//		float fT33 = mtxWorld.m_af[0][1] * m_mtxInverseInertisTensor.m_af[1][0]
//			+ mtxWorld.m_af[1][1] * m_mtxInverseInertisTensor.m_af[1][1]
//			+ mtxWorld.m_af[2][1] * m_mtxInverseInertisTensor.m_af[1][2];
//		float fT38 = mtxWorld.m_af[0][1] * m_mtxInverseInertisTensor.m_af[2][0]
//			+ mtxWorld.m_af[1][1] * m_mtxInverseInertisTensor.m_af[2][1]
//			+ mtxWorld.m_af[2][1] * m_mtxInverseInertisTensor.m_af[2][2];
//		float fT52 = mtxWorld.m_af[0][2] * m_mtxInverseInertisTensor.m_af[0][0]
//			+ mtxWorld.m_af[1][2] * m_mtxInverseInertisTensor.m_af[0][1]
//			+ mtxWorld.m_af[2][2] * m_mtxInverseInertisTensor.m_af[0][2];
//		float fT57 = mtxWorld.m_af[0][2] * m_mtxInverseInertisTensor.m_af[1][0]
//			+ mtxWorld.m_af[1][2] * m_mtxInverseInertisTensor.m_af[1][1]
//			+ mtxWorld.m_af[2][2] * m_mtxInverseInertisTensor.m_af[1][2];
//		float fT62 = mtxWorld.m_af[0][2] * m_mtxInverseInertisTensor.m_af[2][0]
//			+ mtxWorld.m_af[1][2] * m_mtxInverseInertisTensor.m_af[2][1]
//			+ mtxWorld.m_af[2][2] * m_mtxInverseInertisTensor.m_af[2][2];
//
//		mtxIitWorld.m_af[0][0] = fT4 * mtxWorld.m_af[0][0]
//			+ fT9 * mtxWorld.m_af[1][0]
//			+ fT14 * mtxWorld.m_af[2][0];
//		mtxIitWorld.m_af[1][0] = fT4 * mtxWorld.m_af[0][1]
//			+ fT9 * mtxWorld.m_af[1][1]
//			+ fT14 * mtxWorld.m_af[2][1];
//		mtxIitWorld.m_af[2][0] = fT4 * mtxWorld.m_af[0][2]
//			+ fT9 * mtxWorld.m_af[1][2]
//			+ fT14 * mtxWorld.m_af[2][2];
//
//		mtxIitWorld.m_af[0][1] = fT28 * mtxWorld.m_af[0][0]
//			+ fT33 * mtxWorld.m_af[1][0]
//			+ fT38 * mtxWorld.m_af[2][0];
//		mtxIitWorld.m_af[1][1] = fT28 * mtxWorld.m_af[0][1]
//			+ fT33 * mtxWorld.m_af[1][1]
//			+ fT38 * mtxWorld.m_af[2][1];
//		mtxIitWorld.m_af[2][1] = fT28 * mtxWorld.m_af[0][2]
//			+ fT33 * mtxWorld.m_af[1][2]
//			+ fT38 * mtxWorld.m_af[2][2];
//
//		mtxIitWorld.m_af[0][2] = fT52 * mtxWorld.m_af[0][0]
//			+ fT57 * mtxWorld.m_af[1][0]
//			+ fT62 * mtxWorld.m_af[2][0];
//		mtxIitWorld.m_af[1][2] = fT52 * mtxWorld.m_af[0][1]
//			+ fT57 * mtxWorld.m_af[1][1]
//			+ fT62 * mtxWorld.m_af[2][1];
//		mtxIitWorld.m_af[2][2] = fT52 * mtxWorld.m_af[0][2]
//			+ fT57 * mtxWorld.m_af[1][2]
//			+ fT62 * mtxWorld.m_af[2][2];
//	}*/
//}
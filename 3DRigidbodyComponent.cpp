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
#include "physicsSystem.h"
#include "3DRigidbodyComponent.h"
#include "collider.h"
#include "sphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

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
C3DRigidbodyComponent::C3DRigidbodyComponent(GameObject* const pGameObj)
	: CRigidbodyComponent(pGameObj, RB_3D)
	, m_fMass(1.0f)
	, m_fInverseMass(1.0f)
	, m_fDrag(1.0f)
	//, AngularDrag(0.95f)
	, m_fFriction(1.0f)
	, Bounciness(0.0f)
	, GravityCoefficient(5.0f)
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
	m_vAcceleration += PhysicsSystem::Gravity * GravityCoefficient;

	//�͂�������x���v�Z����
	m_vAcceleration += m_vForceAccum * m_fInverseMass;
	
	//��]�͂����]�����x���v�Z����
	//Matrix44 mtxIitWorld;
	//calculateInertiaTensorWorld(mtxIitWorld);
	//Vector3 vAngularAcceleration = CKFMath::Vec3TransformCoord(m_vTorqueAccum, mtxIitWorld);

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
	//m_vTorqueAccum = Vector3(0.0f);
	m_vVelocity *= m_fDrag;
	//m_vAngularVelocity *= AngularDrag;
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
//void C3DRigidbodyComponent::SetInertiaTensor(Collider* pCollider)
//{
//	float fV00 = 1.0f;
//	float fV11 = 1.0f;
//	float fV22 = 1.0f;
//	ColliderType type = pCollider->GetType();
//	if (type == Sphere)
//	{
//		float fRadius = dynamic_cast<SphereCollider*>(pCollider)->GetRadius();
//		fV00 
//			= fV11
//			= fV22
//			= 0.4f * m_fMass * fRadius * fRadius;
//	}
//	else if (type == AABB)
//	{
//		Vector3 vSize = dynamic_cast<AABBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = m_fMass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
//	}
//	else if (type == OBB)
//	{
//		Vector3 vSize = dynamic_cast<OBBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = m_fMass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
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
//void C3DRigidbodyComponent::calculateInertiaTensorWorld(Matrix44& mtxIitWorld)
//{
//	auto listCollider = m_pGameObj->GetColliderComponent();
//	if (!listCollider.empty())
//	{
//		//���[���h�}�g���N�X�̎擾
//		auto itrBegin = listCollider.begin();
//		Matrix44 mtxWorld = (*itrBegin)->GetMatrixWorld();
//		
//		//�ړ��ʂ𖳂���
//		mtxWorld.Elements[3][0] = 0.0f;
//		mtxWorld.Elements[3][1] = 0.0f;
//		mtxWorld.Elements[3][2] = 0.0f;
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
//		const Matrix44& mtxWorld = (*itrBegin)->GetMatrixWorld();
//		float fT4 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT9 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT14 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT28 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT33 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT38 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT52 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT57 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT62 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[2][2];
//
//		mtxIitWorld.Elements[0][0] = fT4 * mtxWorld.Elements[0][0]
//			+ fT9 * mtxWorld.Elements[1][0]
//			+ fT14 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][0] = fT4 * mtxWorld.Elements[0][1]
//			+ fT9 * mtxWorld.Elements[1][1]
//			+ fT14 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][0] = fT4 * mtxWorld.Elements[0][2]
//			+ fT9 * mtxWorld.Elements[1][2]
//			+ fT14 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][1] = fT28 * mtxWorld.Elements[0][0]
//			+ fT33 * mtxWorld.Elements[1][0]
//			+ fT38 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][1] = fT28 * mtxWorld.Elements[0][1]
//			+ fT33 * mtxWorld.Elements[1][1]
//			+ fT38 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][1] = fT28 * mtxWorld.Elements[0][2]
//			+ fT33 * mtxWorld.Elements[1][2]
//			+ fT38 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][2] = fT52 * mtxWorld.Elements[0][0]
//			+ fT57 * mtxWorld.Elements[1][0]
//			+ fT62 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][2] = fT52 * mtxWorld.Elements[0][1]
//			+ fT57 * mtxWorld.Elements[1][1]
//			+ fT62 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][2] = fT52 * mtxWorld.Elements[0][2]
//			+ fT57 * mtxWorld.Elements[1][2]
//			+ fT62 * mtxWorld.Elements[2][2];
//	}*/
//}
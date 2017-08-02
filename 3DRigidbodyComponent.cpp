//--------------------------------------------------------------------------------
//
//�@3DRigidbodyComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "3DRigidbodyComponent.h"
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
C3DRigidbodyComponent::C3DRigidbodyComponent(CGameObject* const pGameObj)
	: CRigidbodyComponent(pGameObj, RB_3D)
	, m_fMass(1.0f)
	, m_fInverseMass(1.0f)
	, m_fDrag(0.98f)
	, m_fAngularDrag(1.0f)
	, m_fFriction(1.0f)
	, m_fBounciness(0.0f)
	, m_vGravity(CKFVec3(0.0f, -0.5f, 0.0f))
	, m_vMovement(CKFVec3(0.0f))
	, m_vVelocity(CKFVec3(0.0f))
	, m_vForceAccum(CKFVec3(0.0f))
	, m_bOnGround(false)
	, m_bRotLock(0)
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
	//�d��
	m_vForceAccum += m_vGravity * m_fMass * DELTA_TIME;

	//�͂�������x���v�Z����
	CKFVec3 vAcceleration = m_vForceAccum * m_fInverseMass;
	
	//��]�͂����]�����x���v�Z����
	CKFVec3 vAngularAcceleration = CKFMath::Vec3TransformCoord(m_vTorqueAccum, m_mtxInverseInertisTensor);

	//���x
	m_vVelocity += vAcceleration;
	m_vAngularVelocity += vAngularAcceleration;

	//�ʒu�X�V
	CTransformComponent* pTrans = m_pGameObj->GetTransformComponent();
	pTrans->MovePosNext(m_vVelocity);
	pTrans->MovePosNext(m_vMovement);

	//��]�X�V
	pTrans->RotByEuler(m_vAngularVelocity);

	//��������
	m_vForceAccum = CKFVec3(0.0f);
	m_vTorqueAccum = CKFVec3(0.0f);
	m_vVelocity *= m_fDrag;
	m_vAngularVelocity *= m_fAngularDrag;
	m_vMovement = CKFVec3(0.0f);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::LateUpdate(void)
{
	//�ʒu�X�V
	m_pGameObj->GetTransformComponent()->MovePosNext(m_vMovement);

	//��������
	m_vMovement = CKFVec3(0.0f);
}
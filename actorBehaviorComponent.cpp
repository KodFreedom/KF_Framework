//--------------------------------------------------------------------------------
//	�������r�w�C�r�A�R���|�l���g
//�@actorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "actorBehaviorComponent.h"
#include "3DRigidbodyComponent.h"
#include "gameObject.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CActorBehaviorComponent::CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody)
	: CBehaviorComponent(pGameObj)
	, m_pRigidbody(pRigidbody)
	, m_nLevel(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, m_fAttack(0.0f)
	, m_fDefence(0.0f)
	, m_fMovementSpeed(0.0f)
	, m_fJumpForce(0.0f)
	, m_fTurnRate(0.0f)
{
}

//--------------------------------------------------------------------------------
//  �ړ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(void)
{

}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(void)
{

}

//--------------------------------------------------------------------------------
//  �U��
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(void)
{

}

//--------------------------------------------------------------------------------
//  ���
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Turn(const CKFVec3& vForward)
{
	CKFVec3 vForwardNext = CKFMath::LerpNormal(m_pGameObj->GetTransformComponent()->GetForwardNext(), vForward, m_fTurnRate);
	m_pGameObj->GetTransformComponent()->RotByForward(vForwardNext);
}
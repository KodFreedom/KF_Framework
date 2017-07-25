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
#include "actorMeshComponent.h"

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
	, m_pAttackCollider(nullptr)
{
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CActorBehaviorComponent::~CActorBehaviorComponent()
{
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Update(void)
{
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::LateUpdate(void)
{
}

//--------------------------------------------------------------------------------
//  �j���[�g�����ɖ߂�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Stay(CActorMeshComponent *pActor)
{
	//�j���[�g�������[�V�����ݒ�
	pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
}

//--------------------------------------------------------------------------------
//  �ړ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(void)
{
	m_pRigidbody->MovePos(m_pGameObj->GetTransformComponent()->GetForwardNext() * m_fMovementSpeed);
}

//--------------------------------------------------------------------------------
//  �ړ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(CActorMeshComponent *pActor)
{
	//�ړ��ݒ�
	Move();

	//�ړ����[�V�����ݒ�
	pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(void)
{
	if (!m_pRigidbody->IsOnGround()) { return; }

	//������ɃW�����v
	m_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * m_fJumpForce * DELTA_TIME);
	m_pRigidbody->SetOnGround(false);
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(CActorMeshComponent *pActor)
{
	if (!m_pRigidbody->IsOnGround()) { return; }

	//�W�����v����
	Jump();

	//�W�����v���[�V�����ݒ�
	pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
}

//--------------------------------------------------------------------------------
//  �U��
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(void)
{

}

//--------------------------------------------------------------------------------
//  �U��
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(CActorMeshComponent *pActor)
{
	//�U������
	Attack();

	//���[�V����
	pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
}

//--------------------------------------------------------------------------------
//  ���
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Turn(const CKFVec3& vForward)
{
	CKFVec3 vForwardNext = CKFMath::LerpNormal(m_pGameObj->GetTransformComponent()->GetForwardNext(), vForward, m_fTurnRate);
	m_pGameObj->GetTransformComponent()->RotByForward(vForwardNext);
}

//--------------------------------------------------------------------------------
//  ���
//--------------------------------------------------------------------------------
void  CActorBehaviorComponent::Turn(const CKFVec3& vForward, CActorMeshComponent *pActor)
{
	//��鏈��
	Turn(vForward);

	//���[�V����

}

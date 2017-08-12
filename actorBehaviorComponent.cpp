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
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CActorBehaviorComponent::CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent& rigidbody, CActorMeshComponent* const pMesh)
	: CBehaviorComponent(pGameObj)
	, m_rigidbody(rigidbody)
	, m_pActor(pMesh)
	, m_nLevel(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, m_fAttack(0.0f)
	, m_fDefence(0.0f)
	, m_fMoveSpeed(0.0f)
	, m_fJumpSpeed(0.0f)
	, m_fTurnSpeedMin(0.0f)
	, m_fTurnSpeedMax(0.0f)
	, m_fGroundCheckDistance(0.0f)
	, m_fAnimSpeed(1.0f)
	, m_bEnabled(true)
	, m_bIsGrounded(false)
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
//  ����������
//--------------------------------------------------------------------------------
bool CActorBehaviorComponent::Init(void)
{
	return true;
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
//	�֐����FAct
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Act(CKFVec3& vMovement, bool& bJump, bool& bAttack)
{
	if (!m_bEnabled) { return; }
	if (!checkCanAction())
	{
		vMovement = CKFVec3(0.0f);
		bJump = false;
		bAttack = false;
	}

	//�����̒�����1���傫���̏ꍇ�m�[�}���C�Y����
	auto fMovement = CKFMath::VecMagnitude(vMovement);
	if (fMovement > 1.0f)
	{//�������x����̂��ߎ�Ōv�Z����
		vMovement /= fMovement;
		fMovement = 1.0f;
	}

	//���Ή�]�������Z�o����
	auto vTurnDir = m_pGameObj->GetTransformComponent()->TransformDirectionToLocal(vMovement);

	//�n�ʂ̕\�ʖ@�����擾
	auto vGroundNormal = checkGroundStatus();

	//�ړ�������n�ʂ̕\�ʂɓ��e����
	vMovement = CKFMath::ProjectOnPlane(vMovement, vGroundNormal);

	//��]�p�x�̎Z�o
	auto fTurnAngle = atan2f(vTurnDir.m_fX, vTurnDir.m_fZ);

	//��]
	turn(fTurnAngle, fMovement);

	//�ړ�
	move(vMovement);

	//����                                                              
	jump(bJump);

	//Animation
	updateAnimation(fMovement, bAttack);
}

void CActorBehaviorComponent::Hit(const float& fDamage)
{

}

//--------------------------------------------------------------------------------
//
//  Protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Fmove
//  �֐������F�ړ��֐�
//	�����F	vMovement�F�ړ������ƈړ���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::move(const CKFVec3& vMovement)
{
	auto vVelocity = vMovement * m_fMoveSpeed;
	vVelocity.m_fY = m_rigidbody.GetVelocity().m_fY;
	m_rigidbody.SetVelocity(vVelocity);
}

//--------------------------------------------------------------------------------
//	�֐����Fjump
//  �֐������F���Ԋ֐�
//	�����F	bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::jump(const bool& bJump)
{
	if (!bJump) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.m_fY = m_fJumpSpeed;
	m_bIsGrounded = false;
	m_fGroundCheckDistance = 0.1f;
}

//--------------------------------------------------------------------------------
//	�֐����Fturn
//  �֐������F��]�֐�
//	�����F	fTurnAngle�F��]�p�x
//			fMoveRate�F�ړ���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::turn(const float& fTurnAngle, const float& fMoveRate)
{
	auto fTurnSpeed = CKFMath::LerpFloat(m_fTurnSpeedMin, m_fTurnSpeedMax, fMoveRate);
	m_pGameObj->GetTransformComponent()->RotByYaw(fTurnAngle * fTurnSpeed);
}

//--------------------------------------------------------------------------------
//	�֐����FupdateAnimation
//  �֐������F�A�j���[�V�����X�V
//	�����F	fTurnAngle�F��]�p�x
//			fMoveRate�F�ړ���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::updateAnimation(const float& fMovement, const bool& bAttack)
{
	if (!m_pActor) { return; }

	if (bAttack && m_pActor->GetMotionNow() != CActorMeshComponent::MOTION_ATTACK)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
		return;
	}

	if (!m_bIsGrounded)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
		return;
	}

	if (fMovement != 0.0f)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
	}
	else
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FcheckGroundStatus
//  �֐������F�����`�F�b�N
//	�����F	�Ȃ�
//	�߂�l�F�n�ʂ̕\�ʖ@��
//--------------------------------------------------------------------------------
CKFVec3 CActorBehaviorComponent::checkGroundStatus(void)
{
	m_bIsGrounded = false;
	return CKFVec3(0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------------------------------------
//	�֐����FcheckCanAction
//  �֐������F�s���ł��邩�ǂ������`�F�b�N����֐�
//	�����F	�Ȃ�
//	�߂�l�F�t���O
//--------------------------------------------------------------------------------
bool CActorBehaviorComponent::checkCanAction(void)
{
	if (m_pActor && m_pActor->GetMotionNow() == CActorMeshComponent::MOTION_ATTACK)
	{
		return false;
	}

	return true;
}
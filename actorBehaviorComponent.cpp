//--------------------------------------------------------------------------------
//	�������r�w�C�r�A�R���|�l���g
//�@actorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "actorBehaviorComponent.h"
#include "rigidbody3D.h"
#include "gameObject.h"
#include "transform.h"
#include "animatorComponent.h"

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
CActorBehaviorComponent::CActorBehaviorComponent(GameObject* const pGameObj, Rigidbody3D& rigidbody, CAnimatorComponent* const pAnimator)
	: CBehaviorComponent(pGameObj)
	, m_rigidbody(rigidbody)
	, m_pAnimator(pAnimator)
	, m_nLevel(0)
	, m_countInvincible(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, Attack(0.0f)
	, m_fDefence(0.0f)
	, m_fMoveSpeed(0.0f)
	, m_fJumpSpeed(0.0f)
	, m_fTurnSpeedMin(0.0f)
	, m_fTurnSpeedMax(0.0f)
	, GroundCheckDistance(0.1f)
	, AnimSpeed(1.0f)
	, m_fMaxPosY(0.0f)
	, m_bEnabled(true)
	, m_bIsGrounded(false)
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
	if (m_countInvincible > 0) { --m_countInvincible; }
	
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
void CActorBehaviorComponent::Act(Vector3& vMovement, const bool& bJump, const bool& bAttack)
{
	if (!m_bEnabled) { return; }
	if (m_pAnimator && !m_pAnimator->CanAct()) { return; }

	//�����̒�����1���傫���̏ꍇ�m�[�}���C�Y����
	auto fMovement = CKFMath::VecMagnitude(vMovement);
	if (fMovement > 1.0f)
	{//�������x����̂��ߎ�Ōv�Z����
		vMovement /= fMovement;
		fMovement = 1.0f;
	}

	//���Ή�]�������Z�o����
	auto vTurnDir = m_pGameObj->GetTransform()->TransformDirectionToLocal(vMovement);

	//�n�ʂ̕\�ʖ@�����擾
	auto vGroundNormal = checkGroundStatus();

	//�ړ�������n�ʂ̕\�ʂɓ��e����
	vMovement = CKFMath::ProjectOnPlane(vMovement, vGroundNormal);

	//��]�p�x�̎Z�o
	auto fTurnAngle = atan2f(vTurnDir.X, vTurnDir.Z);

	//��]
	turn(fTurnAngle, fMovement);

	//�ړ�
	move(vMovement);

	//����                                                              
	jump(bJump);

	//Animation
	updateAnimation(fMovement, bJump, bAttack);
}

//--------------------------------------------------------------------------------
//	�֐����FAct
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Hit(const float& fDamage)
{
	if (m_countInvincible > 0) { return; }
	m_countInvincible = 60;
	m_fLifeNow -= fDamage;
	m_fLifeNow = m_fLifeNow < 0.0f ? 0.0f : m_fLifeNow;
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
void CActorBehaviorComponent::move(const Vector3& vMovement)
{
	auto vVelocity = vMovement * m_fMoveSpeed;
	vVelocity.Y = m_rigidbody.GetVelocity().Y;
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
	if (!bJump || !m_bIsGrounded) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.Y = m_fJumpSpeed;
	m_rigidbody.SetVelocity(vVelocity);
	//GroundCheckDistance = 0.3f;
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
	m_pGameObj->GetTransform()->RotByYaw(fTurnAngle * fTurnSpeed);
}

//--------------------------------------------------------------------------------
//	�֐����FupdateAnimation
//  �֐������F�A�j���[�V�����X�V
//	�����F	fTurnAngle�F��]�p�x
//			fMoveRate�F�ړ���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::updateAnimation(const float& fMovement, const bool& bJump, const bool& bAttack)
{
	if (!m_pAnimator) { return; }

	m_pAnimator->SetGrounded(m_bIsGrounded);
	m_pAnimator->SetAttack(bAttack);
	m_pAnimator->SetJump(bJump);
	m_pAnimator->SetMove(fMovement);
}

//--------------------------------------------------------------------------------
//	�֐����FcheckGroundStatus
//  �֐������F�����`�F�b�N
//	�����F	�Ȃ�
//	�߂�l�F�n�ʂ̕\�ʖ@��
//--------------------------------------------------------------------------------
Vector3 CActorBehaviorComponent::checkGroundStatus(void)
{
	RayHitInfo rayHit;
	auto Position = m_pGameObj->GetTransform()->GetPos();
	auto pCollisionSystem = CollisionSystem::Instance();
	if (pCollisionSystem->RayCast(Position, CKFMath::sc_vDown, GroundCheckDistance, rayHit, m_pGameObj))
	{
		//To do : Jump Damage
		if (!m_bIsGrounded)
		{
			float fFallDis = m_fMaxPosY - Position.Y;
		}
		m_fMaxPosY = Position.Y;
		m_bIsGrounded = true;
		return rayHit.Normal;
	}

	m_fMaxPosY = m_fMaxPosY < Position.Y ? Position.Y : m_fMaxPosY;
	m_bIsGrounded = false;
	return CKFMath::sc_vUp;
}

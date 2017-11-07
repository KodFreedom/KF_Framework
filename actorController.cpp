//--------------------------------------------------------------------------------
//	�������R���g���[��
//�@ActorController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "actorController.h"
#include "gameObjectActor.h"
#include "actorState.h"
#include "animator.h"
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "rigidbody3D.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObjectActor* const owner, const string& name, Rigidbody3D& rigidbody)
	: Behavior(owner, name), currentState(nullptr), rigidbody(rigidbody)
{
	animator = owner->GetAnimator();
}

//--------------------------------------------------------------------------------
//
//  Protected
//
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	�֐����FupdateAnimation
//  �֐������F�A�j���[�V�����X�V
//	�����F	fTurnAngle�F��]�p�x
//			fMoveRate�F�ړ���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorController::updateAnimation(const float& fMovement, const bool& bJump, const bool& bAttack)
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
Vector3 ActorController::checkGroundStatus(void)
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

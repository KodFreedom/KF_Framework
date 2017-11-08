//--------------------------------------------------------------------------------
//	�������X�e�[�g�}�V��
//�@ActorState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "actorState.h"
#include "actorController.h"
#include "gameObject.h"
#include "transform.h"
#include "rigidbody3D.h"
#include "collisionSystem.h"
#include "collisionDetector.h"

//--------------------------------------------------------------------------------
//
//	protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Fmove
//  �֐������F�ړ��֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::move(ActorController& actor)
{
	auto& movement = actor.GetMovement();
	float moveAmount = movement.Magnitude();
	if (moveAmount > 0.0f)
	{
		auto transform = actor.GetGameObject()->GetTransform();
		auto& paramater = actor.GetParamater();

		// ��]
		auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
		direction = Vector3::ProjectOnPlane(direction, currentGroundState.Normal);
		auto rotationY = atan2f(direction.X, direction.Z);
		auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
		transform->RotateByYaw(rotationY * turnSpeed);

		//�ړ�
		transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed());
	}
}

//--------------------------------------------------------------------------------
//	�֐����Fjump
//  �֐������F���Ԋ֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::jump(ActorController& actor)
{
	if (currentGroundState.IsGrounded && actor.GetIsJump())
	{
		auto& rigidbody = actor.GetRigidbody();
		auto velocity = rigidbody.GetVelocity();
		velocity.Y = actor.GetParamater().GetJumpSpeed();
		rigidbody.SetVelocity(velocity);
	}
}

//--------------------------------------------------------------------------------
//	�֐����Fjump
//  �֐������F���Ԋ֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::checkGrounded(const ActorController& actor)
{
	auto& position = actor.GetGameObject()->GetTransform()->GetNextPosition();
	auto rayHitInfo = CollisionSystem::Instance()->RayCast(Ray(position, Vector3::Down), groundCheckDistance, transform->GetGameObject());
	
	if (rayHitInfo)
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
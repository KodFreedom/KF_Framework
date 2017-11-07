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

//--------------------------------------------------------------------------------
//	�֐����FAct
//  �֐������F�s������
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::Act(ActorController& actor)
{
	auto transform = actor.GetGameObject()->GetTransform();
	auto& groundInfo = checkGrounded(*transform);
	auto& paramater = actor.GetParamater();
	
	auto& movement = actor.GetMovement();
	float moveAmount = movement.Magnitude();
	if (moveAmount > 0.0f)
	{
		// ��]
		auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
		direction = Vector3::ProjectOnPlane(direction, groundInfo.Normal);
		auto rotationY = atan2f(direction.X, direction.Z);
		auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
		transform->RotateByYaw(rotationY * turnSpeed);

		//�ړ�
		transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed());
	}
	
	if (groundInfo.IsGrounded && actor.GetIsJump())
	{
		auto& rigidbody = actor.GetRigidbody();
		auto velocity = rigidbody.GetVelocity();
		velocity.Y = paramater.;
		m_rigidbody.SetVelocity(vVelocity);
	}
}

//--------------------------------------------------------------------------------
//	�֐����Fmove
//  �֐������F�ړ��֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::move(ActorController& actor)
{
	auto transform = actor.GetGameObject()->GetTransform();
	auto& groundInfo = checkGrounded(*transform);
	auto& movement = actor.GetMovement();
	float moveAmount = movement.Magnitude();
	if (moveAmount <= 0.0f) return;
	auto& paramater = actor.GetParamater();
	
	// ��]
	auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
	direction = Vector3::ProjectOnPlane(direction, groundInfo.Normal);
	auto rotationY = atan2f(direction.X, direction.Z);
	auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
	transform->RotateByYaw(rotationY * turnSpeed);

	//�ړ�
	transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed());
}

//--------------------------------------------------------------------------------
//	�֐����Fjump
//  �֐������F���Ԋ֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::jump(ActorController& actor)
{
	if (!bJump || !m_bIsGrounded) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.Y = m_fJumpSpeed;
	m_rigidbody.SetVelocity(vVelocity);
	//GroundCheckDistance = 0.3f;
}
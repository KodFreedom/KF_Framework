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
#include "damagedState.h"

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FOnDamaged
//  �֐������F�_���[�W�󂯂��֐�
//	�����F	damage�F�_���[�W��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorState::OnDamaged(ActorController& actor, const float& damage)
{
	auto& paramater = actor.GetParamater();
	auto currentLife = paramater.GetCurrentLife();
	currentLife = Math::Clamp(currentLife - damage, 0.0f, paramater.GetMaxLife());
	paramater.SetCurrentLife(currentLife);
	actor.Change(new DamagedState);
}

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
		moveAmount *= movementMultiplyer;
		auto rotationY = atan2f(direction.X, direction.Z);
		auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
		transform->RotateByYaw(rotationY * turnSpeed * DELTA_TIME);

		//�ړ�
		transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed() * DELTA_TIME);
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
	auto rayHitInfo = CollisionSystem::Instance()->RayCast(Ray(position, Vector3::Down), groundCheckDistance, actor.GetGameObject());
	
	if (rayHitInfo)
	{
		currentGroundState.IsGrounded = true;
		currentGroundState.Normal = rayHitInfo->Normal;
		return;
	}

	currentGroundState.IsGrounded = false;
	currentGroundState.Normal = Vector3::Up;
}
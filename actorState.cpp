//--------------------------------------------------------------------------------
//	生き物ステートマシン
//　ActorState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "actorState.h"
#include "actorController.h"
#include "gameObject.h"
#include "transform.h"
#include "rigidbody3D.h"

//--------------------------------------------------------------------------------
//	関数名：Act
//  関数説明：行動する
//	引数：	actor：アクターコントローラ
//	戻り値：なし
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
		// 回転
		auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
		direction = Vector3::ProjectOnPlane(direction, groundInfo.Normal);
		auto rotationY = atan2f(direction.X, direction.Z);
		auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
		transform->RotateByYaw(rotationY * turnSpeed);

		//移動
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
//	関数名：move
//  関数説明：移動関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorState::move(ActorController& actor)
{
	auto transform = actor.GetGameObject()->GetTransform();
	auto& groundInfo = checkGrounded(*transform);
	auto& movement = actor.GetMovement();
	float moveAmount = movement.Magnitude();
	if (moveAmount <= 0.0f) return;
	auto& paramater = actor.GetParamater();
	
	// 回転
	auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
	direction = Vector3::ProjectOnPlane(direction, groundInfo.Normal);
	auto rotationY = atan2f(direction.X, direction.Z);
	auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
	transform->RotateByYaw(rotationY * turnSpeed);

	//移動
	transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed());
}

//--------------------------------------------------------------------------------
//	関数名：jump
//  関数説明：跳ぶ関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorState::jump(ActorController& actor)
{
	if (!bJump || !m_bIsGrounded) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.Y = m_fJumpSpeed;
	m_rigidbody.SetVelocity(vVelocity);
	//GroundCheckDistance = 0.3f;
}
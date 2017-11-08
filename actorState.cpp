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
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "damagedState.h"

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：OnDamaged
//  関数説明：ダメージ受けた関数
//	引数：	damage：ダメージ量
//	戻り値：なし
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
//	関数名：move
//  関数説明：移動関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorState::move(ActorController& actor)
{
	auto& movement = actor.GetMovement();
	float moveAmount = movement.Magnitude();
	if (moveAmount > 0.0f)
	{
		auto transform = actor.GetGameObject()->GetTransform();
		auto& paramater = actor.GetParamater();

		// 回転
		auto& direction = transform->TransformDirectionToLocal(movement / moveAmount);
		direction = Vector3::ProjectOnPlane(direction, currentGroundState.Normal);
		moveAmount *= movementMultiplyer;
		auto rotationY = atan2f(direction.X, direction.Z);
		auto turnSpeed = Math::Lerp(paramater.GetMaxTurnSpeed(), paramater.GetMaxTurnSpeed(), moveAmount);
		transform->RotateByYaw(rotationY * turnSpeed * DELTA_TIME);

		//移動
		transform->SetNextPosition(transform->GetNextPosition() + movement * paramater.GetMoveSpeed() * DELTA_TIME);
	}
}

//--------------------------------------------------------------------------------
//	関数名：jump
//  関数説明：跳ぶ関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
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
//	関数名：jump
//  関数説明：跳ぶ関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
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
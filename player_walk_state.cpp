//--------------------------------------------------------------------------------
//	歩くステート
//　player_walk_state.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_walk_state.h"
#include "player_neutral_state.h"
#include "player_jump_state.h"
#include "player_damaged_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerWalkState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParameter();
	parameter.SetGroundCheckDistance(kGroundedGroundCheckDistance);
	parameter.SetMovementMultiplier(kGroundedMovementMultiplier);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerWalkState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerWalkState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();
	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		if(actor.GetMovement().SquareMagnitude() <= 0.0f)
		{
			actor.Change(new PlayerNeutralState);
			return;
		}

		if(actor.GetCurrentGroundInfo().is_grounded && actor.IsJump())
		{
			actor.Change(new PlayerJumpState);
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerWalkState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
	if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
	{//武器チェック
		if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
		{
			// Damage
			actor.Change(new PlayerDamagedState);
			return;
		}
	}

	if (other.GetGameObject().GetTag()._Equal(L"Goal"))
	{
		if (self.GetTag()._Equal(L"body"))
		{
			// Game Clear
		}
	}
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerWalkState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
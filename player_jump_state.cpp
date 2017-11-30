//--------------------------------------------------------------------------------
//	ジャンプステート
//　player_jump_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jump_state.h"
#include "player_land_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerJumpState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParamater();
	parameter.SetGroundCheckDistance(kAirborneGroundCheckDistance);
	parameter.SetMovementMultiplier(kAirborneMovementMultiplier);
	auto animator = actor.GetAnimator();
	animator.SetGrounded(false);
	animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerJumpState::Uninit(ActorController& actor)
{
	auto animator = actor.GetAnimator();
	animator.SetGrounded(true);
	animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerJumpState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();
	if (actor.GetCurrentGroundInfo().is_grounded)
	{
		actor.Change(new PlayerLandState);
		return;
	}
	auto& parameter = actor.GetParamater();
	parameter.SetGroundCheckDistance(kAirborneGroundCheckDistance);
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJumpState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
	if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
	{//武器チェック
		if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
		{
			// Damage
			//actor.Change(new PlayerDamagedState);
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
void PlayerJumpState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
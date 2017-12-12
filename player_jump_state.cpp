//--------------------------------------------------------------------------------
//	ジャンプステート
//　player_jump_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jump_state.h"
#include "player_land_state.h"
#include "player_jump_attack_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerJumpState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParameter();
	parameter.SetGroundCheckDistance(kAirborneGroundCheckDistance);
	parameter.SetMovementMultiplier(kAirborneMovementMultiplier);
	auto& animator = actor.GetAnimator();
	animator.SetGrounded(false);
	animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerJumpState::Uninit(ActorController& actor)
{
	auto& animator = actor.GetAnimator();
	animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerJumpState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	
	++frame_counter_;
	if (frame_counter_ == kWaitFrame)
	{
		actor.Jump();
	}
	else if (frame_counter_ > kWaitFrame)
	{
		actor.CheckGrounded();
		actor.Move();

		if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
		{
			if (actor.GetCurrentGroundInfo().is_grounded)
			{
				actor.Change(MY_NEW PlayerLandState);
				return;
			}

			if (actor.IsAttack())
			{
				actor.Change(MY_NEW PlayerJumpAttackState);
				return;
			}
		}
	}
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
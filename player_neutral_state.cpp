//--------------------------------------------------------------------------------
//	待機ステート
//　player_neutral_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_neutral_state.h"
#include "player_walk_state.h"
#include "player_jump_state.h"
#include "player_damaged_state.h"
#include "player_fallen_state.h"
#include "player_attack_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerNeutralState::Init(ActorController& actor)
{
	actor.GetParameter().SetGroundCheckDistance(kNeutralGroundCheckDistance);
	actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerNeutralState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerNeutralState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();

	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		if (actor.GetMovement().SquareMagnitude() > 0.0f)
		{
			actor.Change(MY_NEW PlayerWalkState);
			return;
		}

		if (actor.GetCurrentGroundInfo().is_grounded && actor.IsJump())
		{
			actor.Change(MY_NEW PlayerJumpState);
			return;
		}

		if (!actor.GetCurrentGroundInfo().is_grounded)
		{
			actor.Change(MY_NEW PlayerFallenState);
			return;
		}

		if (actor.IsAttack())
		{
			actor.Change(MY_NEW PlayerAttackState);
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerNeutralState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
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
void PlayerNeutralState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
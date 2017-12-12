//--------------------------------------------------------------------------------
//	攻撃ステート
//　player_attack_state.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_attack_state.h"
#include "player_neutral_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerAttackState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParameter();
	parameter.SetMovementMultiplier(kMovementMultiplier);
	actor.GetAnimator().SetAttack(true);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerAttackState::Uninit(ActorController& actor)
{
	actor.GetAnimator().SetAttack(false);
}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerAttackState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();
	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		const auto& current_animation_name = actor.GetAnimator().GetCurrentAnimationName();
		if (!current_animation_name._Equal(L"jugg_attack_left")
			&& !current_animation_name._Equal(L"jugg_attack_right"))
		{
			actor.Change(MY_NEW PlayerNeutralState);
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
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
void PlayerAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
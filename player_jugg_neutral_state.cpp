//--------------------------------------------------------------------------------
//  jugg待機ステート
//　player_jugg_neutral_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_neutral_state.h"
#include "player_jugg_walk_state.h"
#include "player_jugg_jump_state.h"
#include "player_jugg_damaged_state.h"
#include "player_jugg_fall_state.h"
#include "player_jugg_attack_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Init(ActorController& actor)
{
    actor.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    actor.CheckGrounded();
    actor.Move();

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetMovement().SquareMagnitude() > 0.0f)
        {
            actor.Change(MY_NEW PlayerJuggWalkState);
            return;
        }

        if (actor.GetCurrentGroundInfo().is_grounded && actor.IsJump())
        {
            actor.Change(MY_NEW PlayerJuggJumpState);
            return;
        }

        if (!actor.GetCurrentGroundInfo().is_grounded)
        {
            actor.Change(MY_NEW PlayerJuggFallState);
            return;
        }

        if (actor.IsLightAttack())
        {
            actor.Change(MY_NEW PlayerJuggAttackState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//武器チェック
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            actor.Change(MY_NEW PlayerJuggDamagedState);
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
void PlayerJuggNeutralState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
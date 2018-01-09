//--------------------------------------------------------------------------------
//  jugg攻撃ステート
//　player_jugg_jump_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_jump_attack_state.h"
#include "player_jugg_neutral_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"
#include "../rigidbody3D.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);

    auto& rigidbody = actor.GetRigidbody();
    default_gravity_multiplier_ = rigidbody.GetGravityMultiplier();
    rigidbody.SetGravityMultiplier(kAttackGravityMultiplier);

    actor.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetLightAttack(false);
    actor.GetRigidbody().SetGravityMultiplier(default_gravity_multiplier_);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    actor.CheckGrounded();
    actor.Move();

    if (actor.GetCurrentGroundInfo().is_grounded)
    {
        actor.GetAnimator().SetGrounded(true);
    }

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        const auto& current_animation_name = actor.GetAnimator().GetCurrentAnimationName();
        if (!current_animation_name._Equal(L"jugg_jump_attack"))
        {
            actor.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  jugg攻撃ステート
//　player_jugg_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_attack_state.h"
#include "player_jugg_neutral_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Update(ActorController& actor)
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
            actor.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  jugg落ちるステート
//　player_jugg_fallen_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_fall_state.h"
#include "player_jugg_land_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Uninit(ActorController& actor)
{
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Update(ActorController& actor)
{
    PlayerState::Update(actor);

    actor.CheckGrounded();
    actor.Move();

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetCurrentGroundInfo().is_grounded)
        {
            actor.Change(MY_NEW PlayerJuggLandState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggFallState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggFallState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
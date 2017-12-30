//--------------------------------------------------------------------------------
//  mutant跳ぶステート
//　player_mutant_jump_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_jump_state.h"
#include "player_mutant_land_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kGroundedMovementMultiplier);
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Uninit(ActorController& actor)
{
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(true);
    animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    ++frame_counter_;
    if (frame_counter_ == kWaitFrame)
    {
        actor.Jump();
        actor.GetParameter().SetMovementMultiplier(kAirborneMovementMultiplier);
    }
    else if (frame_counter_ > kWaitFrame)
    {
        actor.CheckGrounded();
        actor.Move();
        if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
        {
            if (actor.GetCurrentGroundInfo().is_grounded)
            {
                actor.Change(MY_NEW PlayerMutantLandState);
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
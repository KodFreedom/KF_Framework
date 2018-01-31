//--------------------------------------------------------------------------------
//  juggジャンプステート
//　player_jugg_jump_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_jump_state.h"
#include "player_jugg_land_state.h"
#include "player_jugg_jump_attack_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    auto& animator = player.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Uninit(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    
    ++frame_counter_;
    if (frame_counter_ == kWaitFrame)
    {
        player.Jump();
    }
    else if (frame_counter_ > kWaitFrame)
    {
        player.CheckGrounded();
        player.Move();

        if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
        {
            if (player.GetCurrentGroundInfo().is_grounded)
            {
                player.Change(MY_NEW PlayerJuggLandState);
                return;
            }

            if (player.IsLightAttack())
            {
                player.Change(MY_NEW PlayerJuggJumpAttackState);
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
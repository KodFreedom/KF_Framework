//--------------------------------------------------------------------------------
//  knight移動状態跳ぶステート
//  player_knight_moving_jump_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_moving_jump_state.h"
#include "player_knight_moving_land_state.h"
#include "player_knight_death_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightMovingJumpState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    auto& animator = player.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
    animator.SetEnableIK(false);
    player.Jump();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightMovingJumpState::Uninit(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    animator.SetGrounded(true);
    animator.SetJump(false);
    animator.SetEnableIK(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightMovingJumpState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerKnightMovingLandState);
            return;
        }
    }

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }
}
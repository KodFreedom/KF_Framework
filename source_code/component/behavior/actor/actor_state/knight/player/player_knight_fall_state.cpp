//--------------------------------------------------------------------------------
//  knight落ちるステート
//  player_knight_fall_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_fall_state.h"
#include "player_knight_idle_state.h"
#include "player_knight_hard_land_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Init(PlayerController& player)
{
    player.GetAnimator().SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    time_counter_ += GameTime::Instance().ScaledDeltaTime();
    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetCurrentGroundInfo().is_grounded)
        {
            if (time_counter_ > kHardLandTime)
            {
                player.Change(MY_NEW PlayerKnightHardLandState);
                return;
            }

            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }
    }
}
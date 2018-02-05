//--------------------------------------------------------------------------------
//  knight走るステート
//  player_run_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_walk_state.h"
#include "player_knight_run_state.h"
#include "player_knight_light_attack_step1_state.h"
#include "player_knight_strong_attack_step1_state.h"
#include "player_knight_dush_attack_state.h"
#include "player_knight_moving_jump_state.h"
#include "player_knight_fall_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightRunState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    //player.GetAnimator().SetEnableIK(false);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightRunState::Uninit(PlayerController& player)
{
    //player.GetAnimator().SetEnableIK(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightRunState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    time_counter_ += GameTime::Instance().ScaledDeltaTime();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetMovement().SquareMagnitude() < 0.5f)
        {
            player.Change(MY_NEW PlayerKnightWalkState);
            return;
        }

        if (player.GetCurrentGroundInfo().is_grounded && player.IsJump())
        {
            player.Change(MY_NEW PlayerKnightMovingJumpState);
            return;
        }

        if (!player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerKnightFallState);
            return;
        }

        if (player.IsLightAttack())
        {
            player.Change(MY_NEW PlayerKnightLightAttackStep1State);
            return;
        }

        if (player.IsStrongAttack())
        {
            if (time_counter_ >= kDushTime)
            {
                player.Change(MY_NEW PlayerKnightDushAttackState);
                return;
            }

            player.Change(MY_NEW PlayerKnightStrongAttackStep1State);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightRunState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}
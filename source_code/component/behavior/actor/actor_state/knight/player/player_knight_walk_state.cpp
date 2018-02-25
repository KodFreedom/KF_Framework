//--------------------------------------------------------------------------------
//  knight歩くステート
//  player_knight_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_walk_state.h"
#include "player_knight_run_state.h"
#include "player_knight_light_attack_step1_state.h"
#include "player_knight_strong_attack_step1_state.h"
#include "player_knight_moving_jump_state.h"
#include "player_knight_fall_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_knight_block_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightWalkState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    //player.GetAnimator().SetEnableIK(false);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightWalkState::Uninit(PlayerController& player)
{
    //player.GetAnimator().SetEnableIK(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightWalkState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        float square_movement = player.GetMovement().SquareMagnitude();
        if (square_movement == 0.0f)
        {
            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }

        if (square_movement > 0.5f)
        {
            player.Change(MY_NEW PlayerKnightRunState);
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
            player.Change(MY_NEW PlayerKnightStrongAttackStep1State);
            return;
        }

        if (player.IsDefence())
        {
            player.Change(MY_NEW PlayerKnightBlockState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightWalkState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}
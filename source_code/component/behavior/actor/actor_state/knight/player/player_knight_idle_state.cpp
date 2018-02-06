//--------------------------------------------------------------------------------
//  knight�ҋ@�X�e�[�g
//�@player_mutant_idle_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_walk_state.h"
#include "player_knight_light_attack_step1_state.h"
#include "player_knight_strong_attack_step1_state.h"
#include "player_knight_standing_jump_state.h"
#include "player_knight_fall_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_knight_block_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightIdleState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightIdleState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetMovement().SquareMagnitude() > 0.0f)
        {
            player.Change(MY_NEW PlayerKnightWalkState);
            return;
        }

        if (player.GetCurrentGroundInfo().is_grounded && player.IsJump())
        {
            player.Change(MY_NEW PlayerKnightStandingJumpState);
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
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void PlayerKnightIdleState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}
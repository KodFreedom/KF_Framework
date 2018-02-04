//--------------------------------------------------------------------------------
//  knight歩くステート
//  player_knight_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_walk_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

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
        if (player.GetMovement().SquareMagnitude() == 0.0f)
        {
            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }

        if (player.GetCurrentGroundInfo().is_grounded && player.IsJump())
        {
            //player.Change(MY_NEW PlayerMutantJumpState);
            return;
        }

        if (!player.GetCurrentGroundInfo().is_grounded)
        {
            //player.Change(MY_NEW PlayerMutantFallState);
            return;
        }

        if (player.IsLightAttack())
        {
            //player.Change(MY_NEW PlayerMutantLightAttackState);
            return;
        }

        if (player.IsStrongAttack())
        {
            //player.Change(MY_NEW PlayerMutantStrongAttackState);
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
        //player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    //player.Change(MY_NEW PlayerMutantDamagedState);
}
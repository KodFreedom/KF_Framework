//--------------------------------------------------------------------------------
//  mutant������X�e�[�g
//�@player_mutant_fall_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_fall_state.h"
#include "player_mutant_land_state.h"
#include "player_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerMutantLandState);
            return;
        }
    }
}
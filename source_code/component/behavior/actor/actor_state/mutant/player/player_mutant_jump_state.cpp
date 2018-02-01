//--------------------------------------------------------------------------------
//  mutant���ԃX�e�[�g
//�@player_mutant_jump_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_jump_state.h"
#include "player_mutant_land_state.h"
#include "player_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kGroundedMovementMultiplier);
    auto& animator = player.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Uninit(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    animator.SetGrounded(true);
    animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    ++frame_counter_;
    if (frame_counter_ == kWaitFrame)
    {
        player.Jump();
        player.GetParameter().SetMovementMultiplier(kAirborneMovementMultiplier);
    }
    else if (frame_counter_ > kWaitFrame)
    {
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
}
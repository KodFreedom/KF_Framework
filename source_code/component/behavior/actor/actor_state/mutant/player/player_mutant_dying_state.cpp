//--------------------------------------------------------------------------------
//  mutant���S�X�e�[�g
//�@player_mutant_dying_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_dying_state.h"
#include "player_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDead(true);
    player.GetAnimator().SetEnableIK(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Update(PlayerController& player)
{
    PlayerState::Update(player);
}
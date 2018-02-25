//--------------------------------------------------------------------------------
//  mutant�_���[�W�󂯂��X�e�[�g
//�@player_mutant_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_damaged_state.h"
#include "../../../../../../game_time.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDamaged(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Update(PlayerController& player)
{
    PlayerState::Update(player);

    time_counter_ += GameTime::Instance().ScaledDeltaTime();
    if (time_counter_ < kWaitTime) return;

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_idle"))
        {
            player.Change(MY_NEW PlayerMutantIdleState);
            return;
        }
    }
}
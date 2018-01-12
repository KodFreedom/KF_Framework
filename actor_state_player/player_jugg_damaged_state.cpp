//--------------------------------------------------------------------------------
//  jugg�_���[�W�󂯂��X�e�[�g
//�@player_jugg_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_damaged_state.h"
#include "player_jugg_neutral_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../time.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Init(PlayerController& player)
{
    player.GetAnimator().SetDamaged(true);
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    time_counter_ -= Time::Instance()->ScaledDeltaTime();
    if (time_counter_ <= 0.0f 
        && player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        player.Change(MY_NEW PlayerJuggNeutralState);
        return;
    }
}
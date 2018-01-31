//--------------------------------------------------------------------------------
//  ���n�X�e�[�g
//�@PlayerJuggLandState
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_land_state.h"
#include "player_jugg_neutral_state.h"
#include "player_jugg_walk_state.h"
#include "player_jugg_jump_state.h"
#include "../player_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Uninit(PlayerController& player)
{
    
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Update(PlayerController& player)
{
    PlayerState::Update(player);

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetMovement().SquareMagnitude() > 0.0f)
        {
            player.Change(MY_NEW PlayerJuggWalkState);
            return;
        }

        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"landing"))
        {
            player.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}
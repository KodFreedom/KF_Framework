//--------------------------------------------------------------------------------
//  ���n�X�e�[�g
//�@PlayerJuggLandState
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_land_state.h"
#include "player_jugg_neutral_state.h"
#include "player_jugg_walk_state.h"
#include "player_jugg_jump_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Init(ActorController& actor)
{
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Uninit(ActorController& actor)
{
    
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggLandState::Update(ActorController& actor)
{
    PlayerState::Update(actor);

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetMovement().SquareMagnitude() > 0.0f)
        {
            actor.Change(MY_NEW PlayerJuggWalkState);
            return;
        }

        if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"landing"))
        {
            actor.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}
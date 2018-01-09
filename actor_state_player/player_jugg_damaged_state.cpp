//--------------------------------------------------------------------------------
//  jugg�_���[�W�󂯂��X�e�[�g
//�@player_jugg_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_damaged_state.h"
#include "player_jugg_neutral_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../time.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Init(ActorController& actor)
{
    actor.GetAnimator().SetDamaged(true);
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggDamagedState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    time_counter_ -= Time::Instance()->ScaledDeltaTime();
    if (time_counter_ <= 0.0f 
        && actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        actor.Change(MY_NEW PlayerJuggNeutralState);
        return;
    }
}
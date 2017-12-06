//--------------------------------------------------------------------------------
//	�_���[�W�󂯂��X�e�[�g
//�@player_damaged_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_damaged_state.h"
#include "player_neutral_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//	�������֐�
//--------------------------------------------------------------------------------
void PlayerDamagedState::Init(ActorController& actor)
{
	actor.GetAnimator().SetDamaged(true);
	actor.GetParameter().SetMovementMultiplier(kDamagedMovementMultiplier);
}

//--------------------------------------------------------------------------------
//	�I������
//--------------------------------------------------------------------------------
void PlayerDamagedState::Uninit(ActorController& actor)
{
	actor.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//	�X�V����
//--------------------------------------------------------------------------------
void PlayerDamagedState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	time_counter_ -= DELTA_TIME;
	if (time_counter_ <= 0.0f 
		&& actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState
		&& !actor.GetAnimator().GetCurrentAnimationName()._Equal(L"unity_chan_damaged"))
	{
		actor.Change(new PlayerNeutralState);
		return;
	}
}
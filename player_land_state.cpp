//--------------------------------------------------------------------------------
//	���n�X�e�[�g
//�@PlayerLandState.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_land_state.h"
#include "player_neutral_state.h"
#include "player_walk_state.h"
#include "player_jump_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//	�������֐�
//--------------------------------------------------------------------------------
void PlayerLandState::Init(ActorController& actor)
{
	actor.GetParamater().SetMovementMultiplier(kLandMovementMultiplier);
}

//--------------------------------------------------------------------------------
//	�I������
//--------------------------------------------------------------------------------
void PlayerLandState::Uninit(ActorController& actor)
{
	
}

//--------------------------------------------------------------------------------
//	�X�V����
//--------------------------------------------------------------------------------
void PlayerLandState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();

	if (actor.GetMovement().SquareMagnitude() > 0.0f)
	{
		actor.Change(new PlayerWalkState);
		return;
	}

	if (actor.IsJump())
	{
		actor.Jump();
		actor.Change(new PlayerJumpState);
		return;
	}

	if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"Landing"))
	{
		actor.Change(new PlayerNeutralState);
		return;
	}
}
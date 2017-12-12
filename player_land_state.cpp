//--------------------------------------------------------------------------------
//	着地ステート
//　PlayerLandState.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_land_state.h"
#include "player_neutral_state.h"
#include "player_walk_state.h"
#include "player_jump_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//	初期化関数
//--------------------------------------------------------------------------------
void PlayerLandState::Init(ActorController& actor)
{
	actor.GetParameter().SetMovementMultiplier(kLandMovementMultiplier);
	actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//	終了処理
//--------------------------------------------------------------------------------
void PlayerLandState::Uninit(ActorController& actor)
{
	
}

//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void PlayerLandState::Update(ActorController& actor)
{
	PlayerState::Update(actor);

	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		if (actor.GetMovement().SquareMagnitude() > 0.0f)
		{
			actor.Change(MY_NEW PlayerWalkState);
			return;
		}

		if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"landing"))
		{
			actor.Change(MY_NEW PlayerNeutralState);
			return;
		}
	}
}
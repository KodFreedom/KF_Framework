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
	actor.GetParamater().SetMovementMultiplier(kLandMovementMultiplier);
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
			actor.Change(new PlayerWalkState);
			return;
		}

		if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"unity_chan_land"))
		{
			actor.Change(new PlayerNeutralState);
			return;
		}
	}
}
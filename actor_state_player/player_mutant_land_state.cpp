//--------------------------------------------------------------------------------
//  mutant着地ステート
//　player_mutant_land_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_land_state.h"
#include "../player_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantLandState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantLandState::Update(PlayerController& player)
{
    PlayerState::Update(player);

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_land"))
        {
            player.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}
//--------------------------------------------------------------------------------
//  mutantダメージ受けたステート
//　player_mutant_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_dying_state.h"
#include "../player_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDamaged(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetParameter().GetCurrentLife() <= 0.0f)
        {
            player.Change(MY_NEW PlayerMutantDyingState);
            return;
        }

        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_damaged"))
        {
            player.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
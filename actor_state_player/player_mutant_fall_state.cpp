//--------------------------------------------------------------------------------
//  mutant落ちるステート
//　player_mutant_fall_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_fall_state.h"
#include "player_mutant_land_state.h"
#include "../player_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Uninit(PlayerController& player)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerMutantLandState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantFallState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantFallState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  knight着地ステート
//  player_knight_standing_land_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_standing_land_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightStandingLandState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightStandingLandState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightStandingLandState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightStandingLandState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}

//--------------------------------------------------------------------------------
//  モーション終了の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightStandingLandState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_standing_land"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}
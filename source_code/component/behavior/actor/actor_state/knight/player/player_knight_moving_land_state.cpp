//--------------------------------------------------------------------------------
//  knight着地ステート
//  player_knight_moving_land_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_run_state.h"
#include "player_knight_moving_land_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightMovingLandState::Init(PlayerController& player)
{

}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightMovingLandState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightMovingLandState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightMovingLandState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        //player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    //player.Change(MY_NEW PlayerMutantDamagedState);
}

//--------------------------------------------------------------------------------
//  モーション終了の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightMovingLandState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_moving_land"))
    {
        player.Change(MY_NEW PlayerKnightRunState);
        return;
    }
}
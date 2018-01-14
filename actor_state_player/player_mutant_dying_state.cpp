//--------------------------------------------------------------------------------
//  mutant死亡ステート
//　player_mutant_dying_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_dying_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../time.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDead(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    time_counter_ -= Time::Instance()->ScaledDeltaTime();
    if (time_counter_ <= 0.0f)
    {
        player.GetGameObject().SetAlive(false);
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
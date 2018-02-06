//--------------------------------------------------------------------------------
//  knight死亡ステート
//  player_knight_death_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "animator.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightDeathState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDead(true);
    player.GetAnimator().SetEnableIK(false);

    // Se
    auto& sound_system = MainSystem::Instance().GetSoundSystem();
    sound_system.Play(kZombieBeatSe);
    sound_system.Play(kDeathVoiceSe);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightDeathState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightDeathState::Update(PlayerController& player)
{
    PlayerState::Update(player);
}
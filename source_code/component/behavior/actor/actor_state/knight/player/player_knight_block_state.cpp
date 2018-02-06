//--------------------------------------------------------------------------------
//  knightブロックステート
//  player_knight_block_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_block_state.h"
#include "player_knight_idle_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDefence(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetDefence(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    player.GetAnimator().SetDefence(player.IsDefence());
}

//--------------------------------------------------------------------------------
//  モーション終了の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_block_impact"))
    {
        player.GetAnimator().SetDamaged(false);
    }

    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_end_block"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnDamaged(PlayerController& player, const float& damage)
{
    player.GetAnimator().SetDamaged(true);
    
    // Se
    auto& sound_system = MainSystem::Instance().GetSoundSystem();
    sound_system.Play(kBlockSe);
    sound_system.Play(kGuardVoiceSe);
}
//--------------------------------------------------------------------------------
//  knightダメージ受けたステート
//  player_knight_impact_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"
#include "game_time.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightImpactState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDamaged(true);

    // Se
    auto& sound_system = MainSystem::Instance().GetSoundSystem();
    sound_system.Play(kZombieBeatSe);
    sound_system.Play(static_cast<SoundEffectLabel>(kDamageVoice1Se + Random::Range(0, 2)));
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightImpactState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightImpactState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    player.GetAnimator().SetDamaged(false);
    time_counter_ += GameTime::Instance().ScaledDeltaTime();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_impact"))
        {
            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerKnightImpactState::OnDamaged(PlayerController& player, const float& damage)
{
    if (time_counter_ <= kInvincibleTime) return;

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
void PlayerKnightImpactState::OnAnimationOver(PlayerController& player)
{
    
}
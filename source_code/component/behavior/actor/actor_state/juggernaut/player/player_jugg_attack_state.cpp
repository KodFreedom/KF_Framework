//--------------------------------------------------------------------------------
//  jugg攻撃ステート
//　player_jugg_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_attack_state.h"
#include "player_jugg_neutral_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        const auto& current_animation_name = player.GetAnimator().GetCurrentAnimationName();
        if (!current_animation_name._Equal(L"jugg_attack_left")
            && !current_animation_name._Equal(L"jugg_attack_right"))
        {
            player.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
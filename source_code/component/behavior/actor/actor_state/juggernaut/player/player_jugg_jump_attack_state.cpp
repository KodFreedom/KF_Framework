//--------------------------------------------------------------------------------
//  jugg攻撃ステート
//　player_jugg_jump_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_jump_attack_state.h"
#include "player_jugg_neutral_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"
#include "../../../../../rigidbody/rigidbody3D.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);

    auto& rigidbody = player.GetRigidbody();
    default_gravity_multiplier_ = rigidbody.GetGravityMultiplier();
    rigidbody.SetGravityMultiplier(kAttackGravityMultiplier);

    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);
    player.GetRigidbody().SetGravityMultiplier(default_gravity_multiplier_);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetCurrentGroundInfo().is_grounded)
    {
        player.GetAnimator().SetGrounded(true);
    }

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        const auto& current_animation_name = player.GetAnimator().GetCurrentAnimationName();
        if (!current_animation_name._Equal(L"jugg_jump_attack"))
        {
            player.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggJumpAttackState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
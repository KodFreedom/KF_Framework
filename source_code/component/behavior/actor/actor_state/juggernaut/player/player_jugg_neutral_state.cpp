//--------------------------------------------------------------------------------
//  jugg待機ステート
//　player_jugg_neutral_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_neutral_state.h"
#include "player_jugg_walk_state.h"
#include "player_jugg_jump_state.h"
#include "player_jugg_damaged_state.h"
#include "player_jugg_fall_state.h"
#include "player_jugg_attack_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Uninit(PlayerController& player)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetMovement().SquareMagnitude() > 0.0f)
        {
            player.Change(MY_NEW PlayerJuggWalkState);
            return;
        }

        if (player.GetCurrentGroundInfo().is_grounded && player.IsJump())
        {
            player.Change(MY_NEW PlayerJuggJumpState);
            return;
        }

        if (!player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerJuggFallState);
            return;
        }

        if (player.IsLightAttack())
        {
            player.Change(MY_NEW PlayerJuggAttackState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//武器チェック
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            player.Change(MY_NEW PlayerJuggDamagedState);
            return;
        }
    }

    if (other.GetGameObject().GetTag()._Equal(L"Goal"))
    {
        if (self.GetTag()._Equal(L"body"))
        {
            // Game Clear
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerJuggNeutralState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
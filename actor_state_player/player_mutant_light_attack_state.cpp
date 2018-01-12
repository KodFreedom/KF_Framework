//--------------------------------------------------------------------------------
//  mutant軽攻撃ステート
//　player_mutant_light_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_light_attack_state.h"
#include "player_mutant_dying_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetParameter().GetCurrentLife() <= 0.0f)
        {
            player.Change(MY_NEW PlayerMutantDyingState);
            return;
        }

        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_punch"))
        {
            player.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//武器チェック
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            player.Change(MY_NEW PlayerMutantDamagedState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  mutant軽攻撃ステート
//　player_mutant_light_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_light_attack_state.h"
#include "player_mutant_dying_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    actor.CheckGrounded();
    actor.Move();

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetParameter().GetCurrentLife() <= 0.0f)
        {
            actor.Change(MY_NEW PlayerMutantDyingState);
            return;
        }

        if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_punch"))
        {
            actor.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//武器チェック
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            actor.Change(MY_NEW PlayerMutantDamagedState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
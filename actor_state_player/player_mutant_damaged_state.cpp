//--------------------------------------------------------------------------------
//  mutantダメージ受けたステート
//　player_mutant_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_dying_state.h"
#include "../actor_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Init(ActorController& actor)
{
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetDamaged(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetDamaged(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetParameter().GetCurrentLife() <= 0.0f)
        {
            actor.Change(MY_NEW PlayerMutantDyingState);
            return;
        }

        if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_damaged"))
        {
            actor.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantDamagedState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
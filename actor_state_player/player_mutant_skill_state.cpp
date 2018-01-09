//--------------------------------------------------------------------------------
//  mutantスキルステート
//　player_mutant_skill_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_skill_state.h"
#include "player_mutant_dying_state.h"
#include "../actor_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetSkill(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetSkill(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Update(ActorController& actor)
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

        if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_skill"))
        {
            actor.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
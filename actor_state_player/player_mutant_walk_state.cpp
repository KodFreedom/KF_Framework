//--------------------------------------------------------------------------------
//  mutant走るステート
//　player_mutant_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_walk_state.h"
#include "player_mutant_jump_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_fall_state.h"
#include "player_mutant_light_attack_state.h"
#include "player_mutant_strong_attack_state.h"
#include "player_mutant_skill_state.h"
#include "player_mutant_dying_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantWalkState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantWalkState::Uninit(PlayerController& player)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantWalkState::Update(PlayerController& player)
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

        if (player.GetMovement().SquareMagnitude() == 0.0f)
        {
            player.Change(MY_NEW PlayerMutantIdelState);
            return;
        }

        if (player.GetCurrentGroundInfo().is_grounded && player.IsJump())
        {
            player.Change(MY_NEW PlayerMutantJumpState);
            return;
        }

        if (!player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerMutantFallState);
            return;
        }

        if (player.IsLightAttack())
        {
            player.Change(MY_NEW PlayerMutantLightAttackState);
            return;
        }

        if (player.IsStrongAttack())
        {
            player.Change(MY_NEW PlayerMutantStrongAttackState);
            return;
        }

        if (player.IsSkill())
        {
            player.Change(MY_NEW PlayerMutantSkillState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantWalkState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
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
void PlayerMutantWalkState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
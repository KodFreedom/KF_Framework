//--------------------------------------------------------------------------------
//  mutant待機ステート
//　player_mutant_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_walk_state.h"
#include "player_mutant_jump_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_fall_state.h"
#include "player_mutant_light_attack_state.h"
#include "player_mutant_strong_attack_state.h"
#include "player_mutant_skill_state.h"
#include "player_mutant_dying_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantIdleState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantIdleState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetMovement().SquareMagnitude() > 0.0f)
        {
            player.Change(MY_NEW PlayerMutantWalkState);
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
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerMutantIdleState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    player.Change(MY_NEW PlayerMutantDamagedState);
}
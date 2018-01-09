//--------------------------------------------------------------------------------
//  mutant�ҋ@�X�e�[�g
//�@player_mutant_idel_state.cpp
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
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantIdelState::Init(ActorController& actor)
{
    actor.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantIdelState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantIdelState::Update(ActorController& actor)
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

        if (actor.GetMovement().SquareMagnitude() > 0.0f)
        {
            actor.Change(MY_NEW PlayerMutantWalkState);
            return;
        }

        if (actor.GetCurrentGroundInfo().is_grounded && actor.IsJump())
        {
            actor.Change(MY_NEW PlayerMutantJumpState);
            return;
        }

        if (!actor.GetCurrentGroundInfo().is_grounded)
        {
            actor.Change(MY_NEW PlayerMutantFallState);
            return;
        }

        if (actor.IsLightAttack())
        {
            actor.Change(MY_NEW PlayerMutantLightAttackState);
            return;
        }

        if (actor.IsStrongAttack())
        {
            actor.Change(MY_NEW PlayerMutantStrongAttackState);
            return;
        }

        if (actor.IsSkill())
        {
            actor.Change(MY_NEW PlayerMutantSkillState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantIdelState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//����`�F�b�N
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            actor.Change(MY_NEW PlayerMutantDamagedState);
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
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantIdelState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  mutant�d�U���X�e�[�g
//�@player_mutant_strong_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idel_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_strong_attack_state.h"
#include "player_mutant_dying_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Init(ActorController& actor)
{
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetStrongAttack(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Uninit(ActorController& actor)
{
    actor.GetAnimator().SetStrongAttack(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Update(ActorController& actor)
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

        if (!actor.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_swiping"))
        {
            actor.Change(MY_NEW PlayerMutantIdelState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
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
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  mutant������X�e�[�g
//�@player_mutant_fall_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_fall_state.h"
#include "player_mutant_land_state.h"
#include "../actor_controller.h"
#include "../animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Init(ActorController& actor)
{
    actor.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantFallState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    actor.CheckGrounded();
    actor.Move();

    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (actor.GetCurrentGroundInfo().is_grounded)
        {
            actor.Change(MY_NEW PlayerMutantLandState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantFallState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantFallState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
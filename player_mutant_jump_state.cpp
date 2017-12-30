//--------------------------------------------------------------------------------
//  mutant���ԃX�e�[�g
//�@player_mutant_jump_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_jump_state.h"
#include "player_mutant_land_state.h"
#include "actor_controller.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kGroundedMovementMultiplier);
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Uninit(ActorController& actor)
{
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(true);
    animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    ++frame_counter_;
    if (frame_counter_ == kWaitFrame)
    {
        actor.Jump();
        actor.GetParameter().SetMovementMultiplier(kAirborneMovementMultiplier);
    }
    else if (frame_counter_ > kWaitFrame)
    {
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
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantJumpState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
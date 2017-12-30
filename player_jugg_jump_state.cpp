//--------------------------------------------------------------------------------
//  jugg�W�����v�X�e�[�g
//�@player_jugg_jump_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_jump_state.h"
#include "player_jugg_land_state.h"
#include "player_jugg_jump_attack_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    auto& animator = actor.GetAnimator();
    animator.SetGrounded(false);
    animator.SetJump(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Uninit(ActorController& actor)
{
    auto& animator = actor.GetAnimator();
    animator.SetJump(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    
    ++frame_counter_;
    if (frame_counter_ == kWaitFrame)
    {
        actor.Jump();
    }
    else if (frame_counter_ > kWaitFrame)
    {
        actor.CheckGrounded();
        actor.Move();

        if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
        {
            if (actor.GetCurrentGroundInfo().is_grounded)
            {
                actor.Change(MY_NEW PlayerJuggLandState);
                return;
            }

            if (actor.IsLightAttack())
            {
                actor.Change(MY_NEW PlayerJuggJumpAttackState);
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggJumpState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
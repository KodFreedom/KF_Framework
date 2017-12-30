//--------------------------------------------------------------------------------
//  �����X�e�[�g
//�@player_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_walk_state.h"
#include "player_jugg_neutral_state.h"
#include "player_jugg_jump_state.h"
#include "player_jugg_damaged_state.h"
#include "player_jugg_fall_state.h"
#include "player_jugg_attack_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggWalkState::Init(ActorController& actor)
{
    auto& parameter = actor.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggWalkState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//    �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggWalkState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    actor.CheckGrounded();
    actor.Move();
    if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if(actor.GetMovement().SquareMagnitude() <= 0.0f)
        {
            actor.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }

        if(actor.GetCurrentGroundInfo().is_grounded && actor.IsJump())
        {
            actor.Change(MY_NEW PlayerJuggJumpState);
            return;
        }

        if (!actor.GetCurrentGroundInfo().is_grounded)
        {
            actor.Change(MY_NEW PlayerJuggFallState);
            return;
        }

        if (actor.IsLightAttack())
        {
            actor.Change(MY_NEW PlayerJuggAttackState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggWalkState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
    if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
    {//����`�F�b�N
        if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
        {
            // Damage
            actor.Change(MY_NEW PlayerJuggDamagedState);
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
void PlayerJuggWalkState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
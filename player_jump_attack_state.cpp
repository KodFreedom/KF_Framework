//--------------------------------------------------------------------------------
//	�U���X�e�[�g
//�@player_attack_state.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jump_attack_state.h"
#include "player_neutral_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"
#include "rigidbody3D.h"

//--------------------------------------------------------------------------------
//	�������֐�
//--------------------------------------------------------------------------------
void PlayerJumpAttackState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParameter();
	parameter.SetMovementMultiplier(kMovementMultiplier);

	auto& rigidbody = actor.GetRigidbody();
	default_gravity_multiplier_ = rigidbody.GetGravityMultiplier();
	rigidbody.SetGravityMultiplier(kAttackGravityMultiplier);

	actor.GetAnimator().SetAttack(true);
}

//--------------------------------------------------------------------------------
//	�I������
//--------------------------------------------------------------------------------
void PlayerJumpAttackState::Uninit(ActorController& actor)
{
	actor.GetAnimator().SetAttack(false);
	actor.GetRigidbody().SetGravityMultiplier(default_gravity_multiplier_);
}

//--------------------------------------------------------------------------------
//	�X�V����
//--------------------------------------------------------------------------------
void PlayerJumpAttackState::Update(ActorController& actor)
{
	PlayerState::Update(actor);
	actor.CheckGrounded();
	actor.Move();

	if (actor.GetCurrentGroundInfo().is_grounded)
	{
		actor.GetAnimator().SetGrounded(true);
	}

	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		const auto& current_animation_name = actor.GetAnimator().GetCurrentAnimationName();
		if (!current_animation_name._Equal(L"jugg_jump_attack"))
		{
			actor.Change(MY_NEW PlayerNeutralState);
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJumpAttackState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
	if (other.GetGameObject().GetTag()._Equal(L"Enemy"))
	{//����`�F�b�N
		if (other.GetTag()._Equal(L"weapon") && self.GetTag()._Equal(L"body"))
		{
			// Damage
			//actor.Change(new PlayerDamagedState);
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
void PlayerJumpAttackState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
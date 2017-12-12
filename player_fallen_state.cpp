//--------------------------------------------------------------------------------
//	������X�e�[�g
//�@player_fallen_state.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_fallen_state.h"
#include "player_land_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//	�������֐�
//--------------------------------------------------------------------------------
void PlayerFallenState::Init(ActorController& actor)
{
	auto& parameter = actor.GetParameter();
	parameter.SetGroundCheckDistance(kAirborneGroundCheckDistance);
	parameter.SetMovementMultiplier(kAirborneMovementMultiplier);
	auto& animator = actor.GetAnimator();
	animator.SetGrounded(false);
}

//--------------------------------------------------------------------------------
//	�I������
//--------------------------------------------------------------------------------
void PlayerFallenState::Uninit(ActorController& actor)
{
	auto& animator = actor.GetAnimator();
	animator.SetGrounded(true);
}

//--------------------------------------------------------------------------------
//	�X�V����
//--------------------------------------------------------------------------------
void PlayerFallenState::Update(ActorController& actor)
{
	PlayerState::Update(actor);

	actor.CheckGrounded();
	actor.Move();

	if (actor.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
	{
		if (actor.GetCurrentGroundInfo().is_grounded)
		{
			actor.Change(MY_NEW PlayerLandState);
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerFallenState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
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
void PlayerFallenState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
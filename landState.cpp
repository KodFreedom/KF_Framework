//--------------------------------------------------------------------------------
//	���n�X�e�[�g
//�@LandState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "landState.h"
#include "walkState.h"
#include "neutralState.h"
#include "jumpState.h"
#include "actorController.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
LandState::LandState()
	: ActorState("Land") {}

//--------------------------------------------------------------------------------
//	�֐����FInit
//  �֐������F�������֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void LandState::Init(ActorController& actor)
{
	auto animator = actor.GetAnimator();
	animator->SetJump(false);
	animator->SetGrounded(true);
}

//--------------------------------------------------------------------------------
//	�֐����FAct
//  �֐������F�s���֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void LandState::Act(ActorController& actor)
{
	if (actor.GetMovement().SquareMagnitude() > 0.0f)
	{
		actor.Change(new WalkState);
		return;
	}

	if (actor.GetIsJump())
	{
		jump(actor);
		actor.Change(new JumpState);
		return;
	}

	if (!actor.GetAnimator()->GetCurrentAnimationName()._Equal("Landing"))
	{
		actor.Change(new NeutralState);
		return;
	}
}
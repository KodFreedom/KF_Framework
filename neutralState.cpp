//--------------------------------------------------------------------------------
//	�ҋ@�X�e�[�g
//�@NeutralState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "neutralState.h"
#include "walkState.h"
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
NeutralState::NeutralState()
	: ActorState("Neutral") {}

//--------------------------------------------------------------------------------
//	�֐����FInit
//  �֐������F�������֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void NeutralState::Init(ActorController& actor)
{
	auto animator = actor.GetAnimator();
	animator->SetAttack(false);
	animator->SetDamaged(false);
	animator->SetMovement(0.0f);
}

//--------------------------------------------------------------------------------
//	�֐����FAct
//  �֐������F�s���֐�
//	�����F	actor�F�A�N�^�[�R���g���[��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void NeutralState::Act(ActorController& actor)
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
}
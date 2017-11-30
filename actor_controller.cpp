//--------------------------------------------------------------------------------
//	�������R���g���[��
//�@ActorController.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "actor_controller.h"
#include "game_object.h"
#include "actor_state.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObject& owner, const String& name, Rigidbody3D& rigidbody, Animator& animator)
	: Behavior(owner, name), state_(nullptr), rigidbody_(rigidbody), animator_(animator)
{}

//--------------------------------------------------------------------------------
//	�֐����FChange
//  �֐������F�X�e�[�g�̐؂�ւ�
//	�����F	state�F�ŐV�̃X�e�[�g
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorController::Change(ActorState* state)
{
	SAFE_DELETE(state_);
	state_ = state;
	state_->Init(*this);
}
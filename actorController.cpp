//--------------------------------------------------------------------------------
//	�������R���g���[��
//�@ActorController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "actorController.h"
#include "gameObjectActor.h"
#include "actorState.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObjectActor* const owner, const string& name, Rigidbody3D& rigidbody)
	: Behavior(owner, name), currentState(nullptr), rigidbody(rigidbody)
{
	animator = owner->GetAnimator();
}

//--------------------------------------------------------------------------------
//	�֐����FChange
//  �֐������F�X�e�[�g�̐؂�ւ�
//	�����F	state�F�ŐV�̃X�e�[�g
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void ActorController::Change(ActorState* state)
{
	SAFE_DELETE(currentState);
	currentState = state;
	currentState->Init(*this);
}
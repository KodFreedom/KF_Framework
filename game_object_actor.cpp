//--------------------------------------------------------------------------------
//	�A�N�^�[
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectActor.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
GameObjectActor::GameObjectActor(const Layer& layer)
	: GameObject(layer)
	, animator(nullptr)
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool GameObjectActor::Init(void)
{
	GameObject::Init();
	if (animator) animator->Init();
	return true;
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void GameObjectActor::LateUpdate(void)
{
	if (!isActive) return;
	GameObject::LateUpdate();
	if (animator) animator->Update();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void GameObjectActor::uninit(void)
{
	GameObject::uninit();
	SAFE_RELEASE(animator);
}
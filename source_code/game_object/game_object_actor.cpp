//--------------------------------------------------------------------------------
//  �A�N�^�[
//�@gameObjectActor.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "game_object_actor.h"
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
{
    animator_ = MY_NEW Animator(*this);
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool GameObjectActor::Init(void)
{
    GameObject::Init();
    animator_->Init();
    return true;
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void GameObjectActor::Update(void)
{
    if (!is_active_) return;
    GameObject::Update();
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void GameObjectActor::LateUpdate(void)
{
    if (!is_active_) return;
    rigidbody_->LateUpdate();
    for (auto& pair : behaviors_) { pair.second->LateUpdate(); }
    animator_->Update();
    transform_->UpdateMatrix();
    for (auto& pair : renderers_) { pair.second->Update(); }
    animator_->LateUpdate();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void GameObjectActor::Uninit(void)
{
    GameObject::Uninit();
    SAFE_RELEASE(animator_);
}
//--------------------------------------------------------------------------------
//
//�@gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#include "game_object.h"
#include "main_system.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
GameObject::GameObject(const Layer& layer)
	: layer_(layer)
	, is_active_(true)
	, is_alive_(true)
{
	transform_ = MY_NEW Transform(*this);
	rigidbody_ = MY_NEW RigidbodyNull(*this);
	MainSystem::Instance()->GetGameObjectManager()->Register(this, layer);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool GameObject::Init(void)
{
	if (!transform_->Init()) { assert("init transform_ error!!"); return false; }
	for (auto& pair : behaviors_) { if (!pair.second->Init()) { assert("init behavior error!!"); return false; } }
	if (!rigidbody_->Init()) { assert("init rigidbody error!!");  return false; }
	for (auto collider : colliders_) { if (!collider->Init()) { assert("init collider error!!");  return false; } }
	for (auto renderer : renderers_) { if (!renderer->Init()) { assert("init render error!!");  return false; } }

	// �q���̏���������
	for (auto& pair : transform_->GetChildren())
	{
		pair.second->GetGameObject().Init();
	}

	return true;
}

//--------------------------------------------------------------------------------
//  �q���̃R���C�_�[���X�g�̎擾
//--------------------------------------------------------------------------------
list<Collider*> GameObject::GetCollidersFromChildren(void) const
{
    list<Collider*> colliders = colliders_;

    // �q������colliders���擾
    for (auto& pair : transform_->GetChildren())
    {
        list<Collider*>& child_colliders = pair.second->GetGameObject().GetCollidersFromChildren();
        colliders.splice(colliders.end(), child_colliders);
    }

    return colliders;
}

//--------------------------------------------------------------------------------
//  SetActive
//--------------------------------------------------------------------------------
void GameObject::SetActive(const bool& value)
{
	if (is_active_ == value) return;
	is_active_ = value;
	if (is_active_)
	{
		transform_->Awake();
		for (auto& pair : behaviors_) pair.second->Awake();
		rigidbody_->Awake();
		for (auto collider : colliders_) collider->Awake();
		for (auto renderer : renderers_) renderer->Awake();
	}
	else
	{
		transform_->Sleep();
		for (auto& pair : behaviors_) pair.second->Sleep();
		rigidbody_->Sleep();
		for (auto collider : colliders_) collider->Sleep();
		for (auto renderer : renderers_) renderer->Sleep();
	}

	for (auto& pair : transform_->GetChildren())
	{
		pair.second->GetGameObject().SetActive(value);
	}
}

//--------------------------------------------------------------------------------
//  SetAlive
//--------------------------------------------------------------------------------
void GameObject::SetAlive(const bool& value)
{
	is_alive_ = value;
	for (auto& pair : transform_->GetChildren())
	{
		pair.second->GetGameObject().SetAlive(value);
	}
}
//--------------------------------------------------------------------------------
//
//　gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#include "main.h"
#include "game_object.h"
#include "main_system.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
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
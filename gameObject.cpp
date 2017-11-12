//--------------------------------------------------------------------------------
//
//�@gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
NullRigidbody	GameObject::nullRigidbody;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
GameObject::GameObject(const Layer& layer)
	: layer(layer)
	, isActive(true)
	, isAlive(true)
	, rigidbody(&nullRigidbody)
{
	name.clear();
	tag.clear();
	behaviors.clear();
	colliders.clear();
	renderers.clear();
	transform = new Transform(this);
	GameObjectManager::Instance()->Register(this, layer);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void GameObject::Update(void)
{
	swapParamater();
	SetActive(CameraManager::Instance()->GetMainCamera()->
		IsInRange(transform->GetCurrentPosition()));
	if (!isActive) return;
	for (auto behavior : behaviors) behavior->Update();
	rigidbody->Update();
	for (auto collider : colliders) collider->Update();
}

//--------------------------------------------------------------------------------
//  SetActive
//--------------------------------------------------------------------------------
void GameObject::SetActive(const bool& value)
{
	if (isActive == value) return;
	isActive = value;
	if (isActive)
	{
		transform->Awake();
		for (auto behavior : behaviors) behavior->Awake();
		rigidbody->Awake();
		for (auto collider : colliders) collider->Awake();
		for (auto renderer : renderers) renderer->Awake();
	}
	else
	{
		transform->Sleep();
		for (auto behavior : behaviors) behavior->Sleep();
		rigidbody->Sleep();
		for (auto collider : colliders) collider->Sleep();
		for (auto renderer : renderers) renderer->Sleep();
	}
	auto& children = transform->GetChildren();
	for (auto child : children)
	{
		child->GetGameObject()->SetActive(value);
	}
}

//--------------------------------------------------------------------------------
//  SetAlive
//--------------------------------------------------------------------------------
void GameObject::SetAlive(const bool& value)
{
	isAlive = value;
	auto& children = transform->GetChildren();
	for (auto child : children)
	{
		child->GetGameObject()->SetAlive(value);
	}
}

//--------------------------------------------------------------------------------
//
//  Protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �p�����[�^�[��������
//--------------------------------------------------------------------------------
void GameObject::swapParamater(void)
{
	transform->SwapParamater();
}
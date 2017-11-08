//--------------------------------------------------------------------------------
//
//　gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
NullRigidbody	GameObject::nullRigidbody;
NullMesh		GameObject::nullMesh;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
GameObject::GameObject(const Layer& layer)
	: layer(layer)
	, isActive(true)
	, isAlive(true)
	, rigidbody(&nullRigidbody)
	, mesh(&nullMesh)
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
//  SetActive
//--------------------------------------------------------------------------------
void GameObject::SetActive(const bool& value)
{
	isActive = value;
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
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void GameObject::swapParamater(void)
{
	transform->SwapParamater();
}
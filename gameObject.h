//--------------------------------------------------------------------------------
//	ゲームオブジェクトスーパークラス
//　gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "transform.h"
#include "behavior.h"
#include "meshRenderer.h"
#include "rigidbody.h"
#include "mesh.h"
#include "collider.h"
#include "gameObjectManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class GameObject
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	GameObject(const Layer& layer = Layer::Default);
	~GameObject() {}
	
	virtual bool	Init(void) 
	{ 
		if (!transform->Init()) { assert("init transform error!!"); return false; }
		for (auto behavior : behaviors) { if (!behavior->Init()) { assert("init behavior error!!"); return false; } }
		if (!rigidbody->Init()) { assert("init rigidbody error!!");  return false; }
		for (auto collider : colliders) { if (!collider->Init()) { assert("init collider error!!");  return false; } }
		if (!mesh->Init()) { assert("init mesh error!!");  return false; }
		for (auto renderer : renderers) { if (!renderer->Init()) { assert("init render error!!");  return false; } }
		return true;
	}
	virtual void	Update(void)
	{
		if (!isActive) return;
		swapParam();
		for (auto behavior : behaviors) { behavior->Update(); }
		rigidbody->Update();
		for (auto collider : colliders) { collider->Update(); }
	}
	virtual void	LateUpdate(void)
	{
		if (!isActive) return;
		rigidbody->LateUpdate();
		for (auto behavior : behaviors) { behavior->LateUpdate(); }
		transform->UpdateMatrix();
		mesh->Update();
		for (auto renderer : renderers) { renderer->Update(); }
	}
	void			Release(void)
	{
		uninit();
		delete this;
	}

	//Get関数
	auto	GetTransform(void) const { return transform; }
	auto&	GetBehaviors(void) const { return behaviors; }
	auto	GetMesh(void) const { return mesh; }
	auto	GetRigidbody(void) const { return rigidbody; }
	auto&	GetColliders(void) const { return colliders; }
	auto&	GetName(void) const { return name; }
	auto&	GetParentName(void) const
	{
		auto parent = transform->GetParent();
		if (!parent) return name;
		return parent->GetGameObject()->GetParentName();
	}
	auto&	GetTag(void) const { return tag; }
	bool	IsAlive(void) const { return isAlive; }
	bool	IsActive(void) const { return isActive; }

	//Set関数
	void	SetMesh(Mesh* const value)
	{
		//Release
		if (mesh != &nullMesh) { SAFE_RELEASE(mesh); }

		//Set
		if (!value) { mesh = &nullMesh; }
		else { mesh = value; }
	}
	void	SetRigidbody(Rigidbody* const value)
	{
		//Release
		if (rigidbody->GetType() != Rigidbody::Type::NullRigidbody) { SAFE_RELEASE(rigidbody); }

		//Set
		if (!value) { rigidbody = &nullRigidbody; }
		else { rigidbody = value; }
	}
	void	SetActive(const bool& value);
	void	SetAlive(const bool& value);
	void	SetName(const string& value) { name = value; }
	void	SetTag(const string& value) { tag = value; }
	void	AddBehavior(Behavior* const behavior) { behaviors.push_back(behavior); }
	void	AddCollider(Collider* const collider) { colliders.push_back(collider); }
	void	AddRenderer(MeshRenderer* const renderer) { renderers.push_back(renderer); }

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	virtual void				swapParamater(void);
	virtual void				uninit(void)
	{
		transform->Release();
		rigidbody->Release();
		mesh->Release();
		for (auto itr = behaviors.begin(); itr != behaviors.end();)
		{
			(*itr)->Release();
			itr = behaviors.erase(itr);
		}
		for (auto itr = colliders.begin(); itr != colliders.end();)
		{
			(*itr)->Release();
			itr = colliders.erase(itr);
		}
		for (auto itr = renderers.begin(); itr != renderers.end();)
		{
			(*itr)->Release();
			itr = renderers.erase(itr);
		}
	}

	//--------------------------------------------------------------------------------
	//  コンポネント
	//--------------------------------------------------------------------------------
	Transform*			transform;
	list<Behavior*>		behaviors;
	Rigidbody*			rigidbody;
	list<Collider*>		colliders;
	Mesh*				mesh;
	list<MeshRenderer*> renderers;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	GameObject(GameObject&) {}
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool	isActive;
	bool	isAlive;
	string	name;
	string	tag;
	Layer	layer;

	//--------------------------------------------------------------------------------
	//  ヌルコンポネント定義
	//--------------------------------------------------------------------------------
	static NullRigidbody	nullRigidbody;
	static NullMesh			nullMesh;
};
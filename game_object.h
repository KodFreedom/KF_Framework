//--------------------------------------------------------------------------------
//	ゲームオブジェクトスーパークラス
//　game_object.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "transform.h"
#include "behavior.h"
#include "mesh_renderer.h"
#include "rigidbody.h"
#include "collider.h"
#include "game_object_manager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class GameObject
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	GameObject(const Layer& layer = kDefaultLayer);
	~GameObject() {}
	
	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual bool Init(void) 
	{ 
		if (!transform_->Init()) { assert("init transform_ error!!"); return false; }
		for (auto& pair : behaviors_) { if (!pair.second->Init()) { assert("init behavior error!!"); return false; } }
		if (!rigidbody_->Init()) { assert("init rigidbody error!!");  return false; }
		for (auto collider : colliders_) { if (!collider->Init()) { assert("init collider error!!");  return false; } }
		for (auto renderer : renderers_) { if (!renderer->Init()) { assert("init render error!!");  return false; } }
		return true;
	}

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	virtual void Update(void)
	{
		if (!is_active_) return;
		for (auto& pair : behaviors_) pair.second->Update();
		rigidbody_->Update();
		for (auto collider : colliders_) collider->Update();
	}

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	virtual void LateUpdate(void)
	{
		if (!is_active_) return;
		rigidbody_->LateUpdate();
		for (auto& pair : behaviors_) { pair.second->LateUpdate(); }
		transform_->UpdateMatrix();
		for (auto renderer : renderers_) { renderer->Update(); }
	}

	//--------------------------------------------------------------------------------
	//  リリース処理
	//--------------------------------------------------------------------------------
	void Release(void)
	{
		Uninit();
		delete this;
	}

	//--------------------------------------------------------------------------------
	//  トランスフォームの取得
	//--------------------------------------------------------------------------------
	const auto GetTransform(void) const { return transform_; }

	//--------------------------------------------------------------------------------
	//  名前でbehaviorの取得
	//--------------------------------------------------------------------------------
	Behavior* const GetBehaviorBy(const String& name)
	{
		auto iterator = behaviors_.find(name);
		if (behaviors_.end() == iterator) { return nullptr; }
		return iterator->second;
	}

	//--------------------------------------------------------------------------------
	//  behaviorsの取得
	//--------------------------------------------------------------------------------
	const auto&	GetBehaviors(void) const { return behaviors_; }

	//--------------------------------------------------------------------------------
	//  リジッドボディの設定
	//--------------------------------------------------------------------------------
	const auto GetRigidbody(void) const { return rigidbody_; }

	//--------------------------------------------------------------------------------
	//  コライダーリストの取得
	//--------------------------------------------------------------------------------
	const auto& GetColliders(void) const { return colliders_; }

	//--------------------------------------------------------------------------------
	//  名前の取得
	//--------------------------------------------------------------------------------
	const String& GetName(void) const { return name_; }

	//--------------------------------------------------------------------------------
	//  親の名前の取得
	//--------------------------------------------------------------------------------
	const String& GetParentName(void) const
	{
		auto parent = transform_->GetParent();
		if (!parent) return name_;
		return parent->GetGameObject().GetParentName();
	}

	//--------------------------------------------------------------------------------
	//  タグの取得
	//--------------------------------------------------------------------------------
	const auto& GetTag(void) const { return tag_; }

	//--------------------------------------------------------------------------------
	//  生きるフラグの取得
	//--------------------------------------------------------------------------------
	const bool& IsAlive(void) const { return is_alive_; }

	//--------------------------------------------------------------------------------
	//  アクティブの取得
	//--------------------------------------------------------------------------------
	const bool& IsActive(void) const { return is_active_; }

	//--------------------------------------------------------------------------------
	//  リジッドボディの設定
	//--------------------------------------------------------------------------------
	void SetRigidbody(Rigidbody* const value)
	{
		if (!value) return;

		//Release
		SAFE_RELEASE(rigidbody_);

		//Set
		rigidbody_ = value;
		rigidbody_->Init();
	}

	//--------------------------------------------------------------------------------
	//  アクティブの設定
	//--------------------------------------------------------------------------------
	void SetActive(const bool& value);

	//--------------------------------------------------------------------------------
	//  生きるフラグの設定
	//--------------------------------------------------------------------------------
	void SetAlive(const bool& value);

	//--------------------------------------------------------------------------------
	//  名前の設定
	//--------------------------------------------------------------------------------
	void SetName(const String& value) { name_ = value; }

	//--------------------------------------------------------------------------------
	//  タグの設定
	//--------------------------------------------------------------------------------
	void SetTag(const String& value) { tag_ = value; }

	//--------------------------------------------------------------------------------
	//  ビヘビアの追加
	//--------------------------------------------------------------------------------
	void AddBehavior(Behavior* const behavior)
	{
		if (!behavior) return;
		behaviors_.emplace(behavior->GetName(), behavior);
	}
	
	//--------------------------------------------------------------------------------
	//  コライダーの追加
	//--------------------------------------------------------------------------------
	void AddCollider(Collider* const collider) 
	{
		if (!collider) return;
		colliders_.push_back(collider);
	}

	//--------------------------------------------------------------------------------
	//  レンダラーの追加
	//--------------------------------------------------------------------------------
	void AddRenderer(MeshRenderer* const renderer) 
	{
		if (!renderer) return;
		renderers_.push_back(renderer);
	}

	//--------------------------------------------------------------------------------
	//  check if this equal value / 同値判定処理
	//  value : 値
	//  return：bool
	//--------------------------------------------------------------------------------
	bool operator==(const GameObject& value) const
	{
		return this == &value;
	}

protected:
	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	virtual void Uninit(void)
	{
		transform_->Release();
		rigidbody_->Release();
		for (auto itr = behaviors_.begin(); itr != behaviors_.end();)
		{
			(*itr)->Release();
			itr = behaviors_.erase(itr);
		}
		for (auto itr = colliders_.begin(); itr != colliders_.end();)
		{
			(*itr)->Release();
			itr = colliders_.erase(itr);
		}
		for (auto itr = renderers_.begin(); itr != renderers_.end();)
		{
			(*itr)->Release();
			itr = renderers_.erase(itr);
		}
	}

	//--------------------------------------------------------------------------------
	//  コンポネント
	//--------------------------------------------------------------------------------
	Transform*                       transform_;
	unordered_map<String, Behavior*> behaviors_;
	Rigidbody*                       rigidbody_;
	list<Collider*>                  colliders_;
	list<MeshRenderer*>              renderers_;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool   is_active_;
	bool   is_alive_;
	String name_;
	String tag_;
	Layer  layer_;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	GameObject(GameObject&) {}
};
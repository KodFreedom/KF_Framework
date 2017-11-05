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
#include "gameObjectManager.h"
#include "transform.h"
#include "behaviorComponent.h"
#include "meshRenderer.h"
#include "rigidbodyComponent.h"
#include "meshComponent.h"
#include "collider.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ゲームオブジェクトクラス
//--------------------------------------------------------------------------------
class GameObject
{
	friend class GameObjectManager;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	GameObject(const Layer& layer = Default);
	~GameObject() {}
	
	virtual bool	Init(void) 
	{ 
		if (!m_pTransform->Init()) { assert("init transform error!!"); return false; }
		for (auto pBehavior : m_listpBehavior) { if (!pBehavior->Init()) { assert("init behavior error!!"); return false; } }
		if (!m_pRigidbody->Init()) { assert("init rigidbody error!!");  return false; }
		for (auto pCollider : m_listpCollider) { if (!pCollider->Init()) { assert("init collider error!!");  return false; } }
		if (!m_mesh->Init()) { assert("init mesh error!!");  return false; }
		if (!m_pRender->Init()) { assert("init render error!!");  return false; }
		return true;
	}
	virtual void	Update(void)
	{
		if (!m_bActive) { return; }
		swapParam();
		for (auto pBehavior : m_listpBehavior) { pBehavior->Update(); }
		m_pRigidbody->Update();
		for (auto pCollider : m_listpCollider) { pCollider->Update(); }
	}
	virtual void	LateUpdate(void)
	{
		if (!m_bActive) { return; }
		m_pRigidbody->LateUpdate();
		for (auto pBehavior : m_listpBehavior) { pBehavior->LateUpdate(); }
		m_mesh->Update();
		m_pTransform->UpdateMatrix();
		m_pRender->Update();
	}
	void			Release(void)
	{
		uninit();
		delete this;
	}

	//Get関数
	auto		GetTransform(void) const { return m_pTransform; }
	auto&		GetBehaviorComponent(void) { return m_listpBehavior; }
	auto		GetMesh(void) const { return m_mesh; }
	auto		GetRigidbodyComponent(void) const { return m_pRigidbody; }
	auto&		GetColliderComponent(void) const { return m_listpCollider; }
	const auto&	GetName(void) const { return m_strName; }
	const auto&	GetParentName(void) const
	{
		auto pParent = m_pTransform;
		while (pParent->GetParent()) { pParent = pParent->GetParent(); }
		return pParent->GetGameObject()->GetName();
	}
	const auto&	GetTag(void) const { return tag; }
	bool		IsActive(void) const { return m_bActive; }

	//Set関数
	void		SetMeshComponent(MeshComponent* mesh) 
	{
		//Release
		if (m_mesh != &s_nullMesh) { SAFE_RELEASE(m_mesh); }

		//Set
		if (!mesh) { m_mesh = &s_nullMesh; }
		else { m_mesh = mesh; }
	}
	void		SetRenderComponent(MeshRenderer* pRender) 
	{
		//Release
		if (m_pRender != &s_nullRender) { SAFE_RELEASE(m_pRender); }

		//Set
		if (!pRender) { m_pRender = &s_nullRender; }
		else { m_pRender = pRender; }
	}
	void		SetRigidbodyComponent(Rigidbody* pRb) 
	{
		//Release
		if (m_pRigidbody != &s_nullRigidbody) { SAFE_RELEASE(m_pRigidbody); }

		//Set
		if (!pRb) { m_pRender = &s_nullRender; }
		else { m_pRigidbody = pRb; }
	}
	void		SetActive(const bool& bActive);
	void		SetAlive(const bool& bAlive);
	void		SetName(const string& strName) { m_strName = strName; }
	void		SetTag(const string& strTag) { tag = strTag; }

	void		AddBehavior(CBehaviorComponent* pBehavior)
	{
		m_listpBehavior.push_back(pBehavior);
	}
	void		AddCollider(Collider* pCollider)
	{
		m_listpCollider.push_back(pCollider);
	}
	void		DeleteCollider(Collider* pCollider)
	{
		m_listpCollider.remove(pCollider);
	}

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	virtual void				swapParam(void);
	virtual void				uninit(void)
	{
		m_pTransform->Release();
		for (auto itr = m_listpBehavior.begin(); itr != m_listpBehavior.end();)
		{
			(*itr)->Release();
			itr = m_listpBehavior.erase(itr);
		}
		m_pRigidbody->Release();
		for (auto itr = m_listpCollider.begin(); itr != m_listpCollider.end();)
		{
			(*itr)->Release();
			itr = m_listpCollider.erase(itr);
		}
		m_mesh->Release();
		m_pRender->Release();
	}

	//--------------------------------------------------------------------------------
	//  コンポネント
	//--------------------------------------------------------------------------------
	Transform*		m_pTransform;	//位置関係パーツ
	list<CBehaviorComponent*>	m_listpBehavior;//行動コンポネント
	Rigidbody*		m_pRigidbody;	//物理処理パーツ
	list<Collider*>	m_listpCollider;//コリジョンパーツ
	MeshComponent*				m_mesh;		//メッシュパーツ
	MeshRenderer*			m_pRender;		//描画処理パーツ

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	GameObject(GameObject&) {}
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool				m_bActive;		//活動フラグ
	bool				m_bAlive;		//生きるフラグ
	string				m_strName;		//オブジェクトの名前
	string				tag;		//オブジェクトのタグ
	Layer			m_layer;		//レイヤ

	//--------------------------------------------------------------------------------
	//  ヌルコンポネント定義
	//--------------------------------------------------------------------------------
	static NullRigidbody	s_nullRigidbody;
	static CNullMeshComponent		s_nullMesh;
	static CNullRenderComponent		s_nullRender;
};
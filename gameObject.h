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
#include "transformComponent.h"
#include "behaviorComponent.h"
#include "renderComponent.h"
#include "rigidbodyComponent.h"
#include "meshComponent.h"
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ゲームオブジェクトクラス
//--------------------------------------------------------------------------------
class CGameObject
{
	friend class CGameObjectManager;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CGameObject(const GOMLAYER& layer = L_DEFAULT);
	~CGameObject() {}
	
	virtual bool	Init(void) 
	{ 
		if (!m_pTransform->Init()) { assert("init transform error!!"); return false; }
		for (auto pBehavior : m_listpBehavior) { if (!pBehavior->Init()) { assert("init behavior error!!"); return false; } }
		if (!m_pRigidbody->Init()) { assert("init rigidbody error!!");  return false; }
		for (auto pCollider : m_listpCollider) { if (!pCollider->Init()) { assert("init collider error!!");  return false; } }
		if (!m_pMesh->Init()) { assert("init mesh error!!");  return false; }
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
		m_pMesh->Update();
		m_pTransform->UpdateMatrix();
		m_pRender->Update();
	}
	void			Release(void)
	{
		uninit();
		delete this;
	}

	//Get関数
	auto		GetTransformComponent(void) const { return m_pTransform; }
	auto&		GetBehaviorComponent(void) { return m_listpBehavior; }
	auto		GetMeshComponent(void) const { return m_pMesh; }
	auto		GetRigidbodyComponent(void) const { return m_pRigidbody; }
	auto&		GetColliderComponent(void) const { return m_listpCollider; }
	const auto&	GetName(void) const { return m_strName; }
	const auto&	GetParentName(void) const
	{
		auto pParent = m_pTransform;
		while (pParent->GetParent()) { pParent = pParent->GetParent(); }
		return pParent->GetGameObject()->GetName();
	}
	const auto&	GetTag(void) const { return m_strTag; }
	bool		IsActive(void) const { return m_bActive; }

	//Set関数
	void		SetMeshComponent(CMeshComponent* pMesh) 
	{
		//Release
		if (m_pMesh != &s_nullMesh) { SAFE_RELEASE(m_pMesh); }

		//Set
		if (!pMesh) { m_pMesh = &s_nullMesh; }
		else { m_pMesh = pMesh; }
	}
	void		SetRenderComponent(CRenderComponent* pRender) 
	{
		//Release
		if (m_pRender != &s_nullRender) { SAFE_RELEASE(m_pRender); }

		//Set
		if (!pRender) { m_pRender = &s_nullRender; }
		else { m_pRender = pRender; }
	}
	void		SetRigidbodyComponent(CRigidbodyComponent* pRb) 
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
	void		SetTag(const string& strTag) { m_strTag = strTag; }

	void		AddBehavior(CBehaviorComponent* pBehavior)
	{
		m_listpBehavior.push_back(pBehavior);
	}
	void		AddCollider(CColliderComponent* pCollider)
	{
		m_listpCollider.push_back(pCollider);
	}
	void		DeleteCollider(CColliderComponent* pCollider)
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
		m_pMesh->Release();
		m_pRender->Release();
	}

	//--------------------------------------------------------------------------------
	//  コンポネント
	//--------------------------------------------------------------------------------
	CTransformComponent*		m_pTransform;	//位置関係パーツ
	list<CBehaviorComponent*>	m_listpBehavior;//行動コンポネント
	CRigidbodyComponent*		m_pRigidbody;	//物理処理パーツ
	list<CColliderComponent*>	m_listpCollider;//コリジョンパーツ
	CMeshComponent*				m_pMesh;		//メッシュパーツ
	CRenderComponent*			m_pRender;		//描画処理パーツ

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CGameObject(CGameObject&) {}
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool				m_bActive;		//活動フラグ
	bool				m_bAlive;		//生きるフラグ
	string				m_strName;		//オブジェクトの名前
	string				m_strTag;		//オブジェクトのタグ
	GOMLAYER			m_layer;		//レイヤ

	//--------------------------------------------------------------------------------
	//  ヌルコンポネント定義
	//--------------------------------------------------------------------------------
	static CNullRigidbodyComponent	s_nullRigidbody;
	static CNullMeshComponent		s_nullMesh;
	static CNullRenderComponent		s_nullRender;
};
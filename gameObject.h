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
#include "drawComponent.h"
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
	friend CGameObjectManager;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CGameObject(const GOM::PRIORITY& pri);
	~CGameObject() {}
	
	virtual bool	Init(void) 
	{ 
		m_pTransform->Init();
		for (auto pBehavior : m_listpBehavior) { pBehavior->Init(); }
		m_pRigidbody->Init();
		for (auto pCollider : m_listpCollider) { pCollider->Init(); }
		m_pMesh->Init();
		m_pDraw->Init();
		return true;
	}
	virtual void	Uninit(void)
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
		m_pDraw->Release();
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
	}
	virtual void	Draw(void)
	{
		if (!m_bActive) { return; }
		m_pDraw->Draw();

#ifdef _DEBUG
		DrawNormal();
#endif
	}
	void			Release(void);

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
	void		SetMeshComponent(CMeshComponent* pMesh) { m_pMesh = pMesh; }
	void		SetDrawComponent(CDrawComponent* pDraw) { m_pDraw = pDraw; }
	void		SetRigidbodyComponent(CRigidbodyComponent* pRb) { m_pRigidbody = pRb; }
	void		SetActive(const bool& bActive);
	void		SetAlive(const bool& bAlive);
	void		SetName(const string& strName) { m_strName = strName; }
	void		SetTag(const string& strTag) { m_strTag = strTag; }

	void		AddBehavior(CBehaviorComponent* pBehavior);
	void		AddCollider(CColliderComponent* pCollider);
	void		DeleteCollider(CColliderComponent* pCollider);

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	virtual void				swapParam(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	//コンポネント
	CTransformComponent*		m_pTransform;	//位置関係パーツ
	list<CBehaviorComponent*>	m_listpBehavior;//行動コンポネント
	CRigidbodyComponent*		m_pRigidbody;	//物理処理パーツ
	list<CColliderComponent*>	m_listpCollider;//コリジョンパーツ
	CMeshComponent*				m_pMesh;		//メッシュパーツ
	CDrawComponent*				m_pDraw;		//描画処理パーツ

private:
#ifdef _DEBUG
	void		DrawNormal(void);
#endif

	//パラメーター(管理用)
	bool				m_bActive;		//活動フラグ
	bool				m_bAlive;		//生きるフラグ
	GOM::PRIORITY		m_pri;			//優先度
	string				m_strName;		//オブジェクトの名前
	string				m_strTag;		//オブジェクトのタグ

	//ヌルコンポネント
	static CNullRigidbodyComponent	s_nullRigidbody;
	static CNullMeshComponent		s_nullMesh;
	static CNullDrawComponent		s_nullDraw;
};
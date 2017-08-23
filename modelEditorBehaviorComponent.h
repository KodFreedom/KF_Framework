//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　modelEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModelEditorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CModelEditorBehaviorComponent(CGameObject* const pGameObj);
	~CModelEditorBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

	//Set関数
	void	SetActive(const bool& bActive) { m_bActive = bActive; }
	void	SetPos(const CKFVec3& vPos);

private:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum MODEL_TYPE
	{
		MT_BOX,
		MT_TREE,
		MT_PINE_TREE,
		MT_ROCK_1,
		MT_ROCK_2,
		MT_ROCK_3,
		MT_ROCK_4,
		MT_BAKER_HOUSE,
		MT_MAX
	};

	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const float sc_fRotSpeed;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	create(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	vector<CGameObject*>	m_vecpObjDemo;
	list<CGameObject*>		m_listObjCreated;
	MODEL_TYPE				m_modelType;
	bool					m_bActive;
};
//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class CEnemyState;
class CNormalEnemyState;
class CAttackEnemyState;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  プレイヤー行動コンポネントクラス
//--------------------------------------------------------------------------------
class CEnemyBehaviorComponent : public CBehaviorComponent
{
	friend CNormalEnemyState;
	friend CAttackEnemyState;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CEnemyBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed = 0.05f, const float& fJumpForce = 20.0f);
	~CEnemyBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, const CColliderComponent& collider) override;
	void	OnCollision(CColliderComponent& colliderThis, const CCollisionInfo& collisionInfo) override;

	//Get関数
	float	GetLifeMax(void) const { return c_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }
	
	void	ChangeState(CEnemyState* pEnemyState);

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	const float						c_fSpeed;
	const float						c_fJumpForce;
	C3DRigidbodyComponent* const	c_pRigidbody;
	const float						c_fLifeMax = 100.0f;
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	float			m_fLifeNow;
	CGameObject*	m_pTarget;
	CEnemyState*	m_pState;
};

//--------------------------------------------------------------------------------
//  エネミー状態
//--------------------------------------------------------------------------------
class CEnemyState
{
public:
	CEnemyState() {}
	~CEnemyState() {}

	virtual void Update(CEnemyBehaviorComponent& enemy) = 0;
};

//--------------------------------------------------------------------------------
//  普通状態
//--------------------------------------------------------------------------------
class CNormalEnemyState : public CEnemyState
{
public:
	CNormalEnemyState() : CEnemyState() {}
	~CNormalEnemyState() {}

	void Update(CEnemyBehaviorComponent& enemy) override;
};

//--------------------------------------------------------------------------------
//  攻撃状態
//--------------------------------------------------------------------------------
class CAttackEnemyState : public CEnemyState
{
public:
	CAttackEnemyState() : CEnemyState() {}
	~CAttackEnemyState() {}

	void Update(CEnemyBehaviorComponent& enemy) override;
};
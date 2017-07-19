//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerBehaviorComponent.h
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

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  プレイヤー行動コンポネントクラス
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed = 0.075f, const float& fJumpForce = 20.0f);
	~CPlayerBehaviorComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override {}
			
	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override;
	void	OnCollision(CColliderComponent& colliderThis, CCollisionInfo& collisionInfo) override;

	//Get関数
	float	GetLifeMax(void) const { return c_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

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
	float	m_fLifeNow;
	unsigned int		m_usCntWhosYourDaddy;
	CColliderComponent*	m_pAttackCollider;
};
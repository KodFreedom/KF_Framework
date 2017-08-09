//--------------------------------------------------------------------------------
//	生き物ビヘイビアコンポネント
//　actorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
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
class CActorMeshComponent;
//class CStatus;
//class CPlayerNormalStatus;
//class CPlayerMoveStatus;
//class CPlayerJumpStatus;
//class CPlayerAttackStatus;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生き物行動コンポネントクラス
//--------------------------------------------------------------------------------
class CActorBehaviorComponent : public CBehaviorComponent
{
	//--------------------------------------------------------------------------------
	//  フレンドクラス
	//--------------------------------------------------------------------------------
	//friend CPlayerNormalStatus;
	//friend CPlayerMoveStatus;
	//friend CPlayerJumpStatus;
	//friend CPlayerAttackStatus;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody);
	~CActorBehaviorComponent();

	virtual bool	Init(void) override = 0;
	virtual void	Uninit(void) override;
	virtual void	Update(void) override;
	virtual void	LateUpdate(void) override;

	virtual void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override = 0;
	virtual void	OnCollision(CCollisionInfo& collisionInfo) override = 0;

	//Set関数
	void	SetLevel(const int& nLevel) { m_nLevel = nLevel; }
	void	SetLifeMax(const float& fLifeMax) { m_fLifeMax = fLifeMax; }
	void	SetLifeNow(const float& fLifeNow) { m_fLifeNow = fLifeNow; }
	void	SetAttack(const float& fAttack) { m_fAttack = fAttack; }
	void	SetDefence(const float& fDefence) { m_fDefence = fDefence; }
	void	SetMovementSpeed(const float& fMovementSpeed) { m_fMovementSpeed = fMovementSpeed; }
	void	SetJumpForce(const float& fJumpForce) { m_fJumpForce = fJumpForce; }
	void	SetTurnRate(const float& fTurnRate) { m_fTurnRate = fTurnRate; }

	//Get関数
	float	GetLifeMax(void) const { return m_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

protected:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	void	Stay(CActorMeshComponent* pActor);
	void	Move(void);				  
	void	Move(CActorMeshComponent* pActor);
	void	Jump(void);				  
	void	Jump(CActorMeshComponent* pActor);
	void	Attack(void);
	void	Attack(CActorMeshComponent* pActor);
	void	Turn(const CKFVec3& vForward);
	void	Turn(const CKFVec3& vForward, CActorMeshComponent *pActor);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	C3DRigidbodyComponent*	m_pRigidbody;	//リジッドボディ

	//一時採用
	CColliderComponent*		m_pAttackCollider;

	//パラメーター
	int		m_nLevel;			//レベル
	float	m_fLifeMax;			//最大生命値
	float	m_fLifeNow;			//今の生命値
	float	m_fAttack;			//攻撃力
	float	m_fDefence;			//防御力
	float	m_fMovementSpeed;	//移動速度
	float	m_fJumpForce;		//跳ぶ力
	float	m_fTurnRate;		//回る速度、pi(180度)回るに必要な時間(秒)
};
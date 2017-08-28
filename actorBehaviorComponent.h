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
class CAnimatorComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生き物行動コンポネントクラス
//--------------------------------------------------------------------------------
class CActorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent& rigidbody, CAnimatorComponent* const pAnimator);
	~CActorBehaviorComponent();

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void) override;
	virtual void	LateUpdate(void) override;

	virtual void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	virtual void	OnCollision(CCollisionInfo& collisionInfo) override {}

	void	Act(CKFVec3& vMovement, const bool& bJump, const bool& bAttack);
	void	Hit(const float& fDamage);

	//Set関数
	void	SetLevel(const int& nLevel) { m_nLevel = nLevel; }
	void	SetLifeMax(const float& fLifeMax) { m_fLifeMax = fLifeMax; }
	void	SetLifeNow(const float& fLifeNow) { m_fLifeNow = fLifeNow; }
	void	SetAttack(const float& fAttack) { m_fAttack = fAttack; }
	void	SetDefence(const float& fDefence) { m_fDefence = fDefence; }
	void	SetMoveSpeed(const float& fMoveSpeed) { m_fMoveSpeed = fMoveSpeed; }
	void	SetJumpSpeed(const float& fJumpSpeed) { m_fJumpSpeed = fJumpSpeed; }
	void	SetTurnSpeedMin(const float& fTurnSpeed) { m_fTurnSpeedMin = fTurnSpeed; }
	void	SetTurnSpeedMax(const float& fTurnSpeed) { m_fTurnSpeedMax = fTurnSpeed; }
	
	//Get関数
	float	GetLifeMax(void) const { return m_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

protected:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	void	move(const CKFVec3& vMovement);
	void	jump(const bool& bJump);
	void	turn(const float& fTurnAngle, const float& fMoveRate);
	void	updateAnimation(const float& fMovement, const bool& bJump, const bool& bAttack);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	C3DRigidbodyComponent&	m_rigidbody;	//リジッドボディ
	CAnimatorComponent*		m_pAnimator;

	//一時採用
	CColliderComponent*		m_pAttackCollider;

	//パラメーター
	int		m_nLevel;				//レベル
	int		m_nCntFalling;
	int		m_nCntInvincible;		//無敵時間
	float	m_fLifeMax;				//最大生命値
	float	m_fLifeNow;				//今の生命値
	float	m_fAttack;				//攻撃力
	float	m_fDefence;				//防御力
	float	m_fMoveSpeed;			//移動速度
	float	m_fJumpSpeed;			//跳ぶ力
	float	m_fTurnSpeedMin;		//回る速度
	float	m_fTurnSpeedMax;
	float	m_fGroundCheckDistance;
	float	m_fAnimSpeed;
	bool	m_bEnabled;
	bool	m_bIsGrounded;

private:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CKFVec3 checkGroundStatus(void);
};
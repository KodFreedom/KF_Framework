//--------------------------------------------------------------------------------
//	�G�l�~�[�r�w�C�r�A�R���|�l���g
//�@enemyBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class CEnemyState;
class CNormalEnemyState;
class CAttackEnemyState;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CEnemyBehaviorComponent : public CBehaviorComponent
{
	friend CNormalEnemyState;
	friend CAttackEnemyState;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CEnemyBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed = 0.05f, const float& fJumpForce = 20.0f);
	~CEnemyBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, const CColliderComponent& collider) override;
	void	OnCollision(CColliderComponent& colliderThis, const CCollisionInfo& collisionInfo) override;

	//Get�֐�
	float	GetLifeMax(void) const { return c_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }
	
	void	ChangeState(CEnemyState* pEnemyState);

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	const float						c_fSpeed;
	const float						c_fJumpForce;
	C3DRigidbodyComponent* const	c_pRigidbody;
	const float						c_fLifeMax = 100.0f;
	
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	float			m_fLifeNow;
	CGameObject*	m_pTarget;
	CEnemyState*	m_pState;
};

//--------------------------------------------------------------------------------
//  �G�l�~�[���
//--------------------------------------------------------------------------------
class CEnemyState
{
public:
	CEnemyState() {}
	~CEnemyState() {}

	virtual void Update(CEnemyBehaviorComponent& enemy) = 0;
};

//--------------------------------------------------------------------------------
//  ���ʏ��
//--------------------------------------------------------------------------------
class CNormalEnemyState : public CEnemyState
{
public:
	CNormalEnemyState() : CEnemyState() {}
	~CNormalEnemyState() {}

	void Update(CEnemyBehaviorComponent& enemy) override;
};

//--------------------------------------------------------------------------------
//  �U�����
//--------------------------------------------------------------------------------
class CAttackEnemyState : public CEnemyState
{
public:
	CAttackEnemyState() : CEnemyState() {}
	~CAttackEnemyState() {}

	void Update(CEnemyBehaviorComponent& enemy) override;
};
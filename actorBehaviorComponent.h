//--------------------------------------------------------------------------------
//	�������r�w�C�r�A�R���|�l���g
//�@actorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
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
class CAnimatorComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �������s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CActorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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

	//Set�֐�
	void	SetLevel(const int& nLevel) { m_nLevel = nLevel; }
	void	SetLifeMax(const float& fLifeMax) { m_fLifeMax = fLifeMax; }
	void	SetLifeNow(const float& fLifeNow) { m_fLifeNow = fLifeNow; }
	void	SetAttack(const float& fAttack) { m_fAttack = fAttack; }
	void	SetDefence(const float& fDefence) { m_fDefence = fDefence; }
	void	SetMoveSpeed(const float& fMoveSpeed) { m_fMoveSpeed = fMoveSpeed; }
	void	SetJumpSpeed(const float& fJumpSpeed) { m_fJumpSpeed = fJumpSpeed; }
	void	SetTurnSpeedMin(const float& fTurnSpeed) { m_fTurnSpeedMin = fTurnSpeed; }
	void	SetTurnSpeedMax(const float& fTurnSpeed) { m_fTurnSpeedMax = fTurnSpeed; }
	
	//Get�֐�
	float	GetLifeMax(void) const { return m_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	void	move(const CKFVec3& vMovement);
	void	jump(const bool& bJump);
	void	turn(const float& fTurnAngle, const float& fMoveRate);
	void	updateAnimation(const float& fMovement, const bool& bJump, const bool& bAttack);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	C3DRigidbodyComponent&	m_rigidbody;	//���W�b�h�{�f�B
	CAnimatorComponent*		m_pAnimator;

	//�ꎞ�̗p
	CColliderComponent*		m_pAttackCollider;

	//�p�����[�^�[
	int		m_nLevel;				//���x��
	int		m_nCntFalling;
	int		m_nCntInvincible;		//���G����
	float	m_fLifeMax;				//�ő吶���l
	float	m_fLifeNow;				//���̐����l
	float	m_fAttack;				//�U����
	float	m_fDefence;				//�h���
	float	m_fMoveSpeed;			//�ړ����x
	float	m_fJumpSpeed;			//���ԗ�
	float	m_fTurnSpeedMin;		//��鑬�x
	float	m_fTurnSpeedMax;
	float	m_fGroundCheckDistance;
	float	m_fAnimSpeed;
	bool	m_bEnabled;
	bool	m_bIsGrounded;

private:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	CKFVec3 checkGroundStatus(void);
};
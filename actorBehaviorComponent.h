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
class CActorMeshComponent;
//class CStatus;
//class CPlayerNormalStatus;
//class CPlayerMoveStatus;
//class CPlayerJumpStatus;
//class CPlayerAttackStatus;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �������s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CActorBehaviorComponent : public CBehaviorComponent
{
	//--------------------------------------------------------------------------------
	//  �t�����h�N���X
	//--------------------------------------------------------------------------------
	//friend CPlayerNormalStatus;
	//friend CPlayerMoveStatus;
	//friend CPlayerJumpStatus;
	//friend CPlayerAttackStatus;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody);
	~CActorBehaviorComponent();

	virtual bool	Init(void) override = 0;
	virtual void	Uninit(void) override;
	virtual void	Update(void) override;
	virtual void	LateUpdate(void) override;

	virtual void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override = 0;
	virtual void	OnCollision(CCollisionInfo& collisionInfo) override = 0;

	//Set�֐�
	void	SetLevel(const int& nLevel) { m_nLevel = nLevel; }
	void	SetLifeMax(const float& fLifeMax) { m_fLifeMax = fLifeMax; }
	void	SetLifeNow(const float& fLifeNow) { m_fLifeNow = fLifeNow; }
	void	SetAttack(const float& fAttack) { m_fAttack = fAttack; }
	void	SetDefence(const float& fDefence) { m_fDefence = fDefence; }
	void	SetMovementSpeed(const float& fMovementSpeed) { m_fMovementSpeed = fMovementSpeed; }
	void	SetJumpForce(const float& fJumpForce) { m_fJumpForce = fJumpForce; }
	void	SetTurnRate(const float& fTurnRate) { m_fTurnRate = fTurnRate; }

	//Get�֐�
	float	GetLifeMax(void) const { return m_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐��錾
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
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	C3DRigidbodyComponent*	m_pRigidbody;	//���W�b�h�{�f�B

	//�ꎞ�̗p
	CColliderComponent*		m_pAttackCollider;

	//�p�����[�^�[
	int		m_nLevel;			//���x��
	float	m_fLifeMax;			//�ő吶���l
	float	m_fLifeNow;			//���̐����l
	float	m_fAttack;			//�U����
	float	m_fDefence;			//�h���
	float	m_fMovementSpeed;	//�ړ����x
	float	m_fJumpForce;		//���ԗ�
	float	m_fTurnRate;		//��鑬�x�Api(180�x)���ɕK�v�Ȏ���(�b)
};
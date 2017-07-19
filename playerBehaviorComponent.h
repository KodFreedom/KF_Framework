//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@playerBehaviorComponent.h
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

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed = 0.075f, const float& fJumpForce = 20.0f);
	~CPlayerBehaviorComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override {}
			
	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override;
	void	OnCollision(CColliderComponent& colliderThis, CCollisionInfo& collisionInfo) override;

	//Get�֐�
	float	GetLifeMax(void) const { return c_fLifeMax; }
	float	GetLifeNow(void) const { return m_fLifeNow; }

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
	float	m_fLifeNow;
	unsigned int		m_usCntWhosYourDaddy;
	CColliderComponent*	m_pAttackCollider;
};
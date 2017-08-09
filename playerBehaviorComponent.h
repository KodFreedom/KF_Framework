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
#include "actorBehaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
//class C3DRigidbodyComponent;
//class CActorMeshComponent;
class CStatus;
class CPlayerNormalStatus;
class CPlayerMoveStatus;
class CPlayerJumpStatus;
class CPlayerAttackStatus;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent : public CActorBehaviorComponent
{
	//--------------------------------------------------------------------------------
	//  �t�����h�N���X
	//--------------------------------------------------------------------------------
	friend CPlayerNormalStatus;
	friend CPlayerMoveStatus;
	friend CPlayerJumpStatus;
	friend CPlayerAttackStatus;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody);
	~CPlayerBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
			
	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override;
	void	OnCollision(CCollisionInfo& collisionInfo) override;

	//��Ԃ̐؂�ւ�
	void	ChangeStatus(CStatus* const pStatus);

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CStatus*	m_pStatus;		//���

	unsigned int	m_usCntWhosYourDaddy;
};
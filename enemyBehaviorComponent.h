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
class CActorBehaviorComponent;
class CAIMode;
class CEnemyNormalMode;
class CEnemyAttackMode;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CEnemyBehaviorComponent : public CBehaviorComponent
{
	friend CEnemyNormalMode;
	friend CEnemyAttackMode;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CEnemyBehaviorComponent(GameObject* const pGameObj, CActorBehaviorComponent& actor);
	~CEnemyBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;

	void	OnTrigger(Collider& colliderThis, Collider& collider) override;
	void	OnCollision(CollisionInfo& collisionInfo) override;

	void	ChangeMode(CAIMode* pAIMode);
private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	GameObject*				m_pTarget;
	CAIMode*					m_pMode;
	CActorBehaviorComponent&	m_actor;
};
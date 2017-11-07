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
#include "behavior.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class ActorController;
class CAIMode;
class CEnemyNormalMode;
class CEnemyAttackMode;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class EnemyController : public Behavior
{
	friend CEnemyNormalMode;
	friend CEnemyAttackMode;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	EnemyController(GameObject* const pGameObj, ActorController& actor);
	~EnemyController() {}

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
	ActorController&	m_actor;
};
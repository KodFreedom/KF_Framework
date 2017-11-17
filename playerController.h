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
#include "actorController.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PlayerController : public ActorController
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	PlayerController(GameObjectActor* const owner, Rigidbody3D& rigidbody);
	~PlayerController() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
	void	OnTrigger(Collider& colliderThis, Collider& collider) override;
	void	OnCollision(CollisionInfo& collisionInfo) override;
};
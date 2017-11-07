//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@WindmillController.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behavior.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class WindmillController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	WindmillController(GameObject* const owner)
		: Behavior(owner, "WindmillController")
		, rotateSpeed(0.01f)
		, fan(nullptr)
	{}
	~WindmillController() {}

	bool	Init(void) override;
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(Collider& colliderThis, Collider& collider) override {}
	void	OnCollision(CollisionInfo& collisionInfo) override {}

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	float		rotateSpeed;
	GameObject*	fan;
};
//--------------------------------------------------------------------------------
//	�������R���g���[��
//�@ActorController.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behavior.h"
#include "actorParamater.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObjectActor;
class ActorState;
class Animator;
class Rigidbody3D;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class ActorController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	ActorController(GameObjectActor* const owner, const string& name, Rigidbody3D& rigidbody);
	~ActorController() {}

	virtual bool Init(void) override = 0;
	virtual void Uninit(void) override = 0;
	virtual void Update(void) override = 0;
	virtual void LateUpdate(void) override = 0;
	virtual void OnTrigger(Collider& colliderThis, Collider& collider) override = 0;
	virtual void OnCollision(CollisionInfo& collisionInfo) override = 0;

	void		 Change(ActorState* state);
	auto		 GetAnimator(void) { return animator; }
	auto&		 GetParamater(void) { return paramater; }
	Rigidbody3D& GetRigidbody(void) { return rigidbody; }
	auto		 GetMovement(void) { return movement; }
	auto		 GetIsJump(void) { return isJump; }
	auto		 GetIsAttack(void) { return isAttack; }

protected:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Animator*		animator;
	ActorState*		currentState;
	Rigidbody3D&	rigidbody;
	ActorParamater  paramater;
	Vector3			movement;
	bool			isAttack;
	bool			isJump;
};
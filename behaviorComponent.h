//--------------------------------------------------------------------------------
//	�r�w�C�r�A�R���|�l���g
//�@behaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-16
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CollisionInfo;
class Collider;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CBehaviorComponent : public Component
{
public:
	CBehaviorComponent(GameObject* const pGameObj) : Component(pGameObj) {}
	~CBehaviorComponent() {}

	virtual bool Init(void) override = 0;
	virtual void Uninit(void) override = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
				 
	virtual void OnTrigger(Collider& colliderThis, Collider& collider) = 0;
	virtual void OnCollision(CollisionInfo& collisionInfo) = 0;
};
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
class CCollisionInfo;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CBehaviorComponent : public CComponent
{
public:
	CBehaviorComponent(CGameObject* const pGameObj) : CComponent(pGameObj) {}
	~CBehaviorComponent() {}

	virtual bool Init(void) override = 0;
	virtual void Uninit(void) override = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
				 
	virtual void OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) = 0;
	virtual void OnCollision(CCollisionInfo& collisionInfo) = 0;
};
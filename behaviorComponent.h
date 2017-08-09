//--------------------------------------------------------------------------------
//	ビヘイビアコンポネント
//　behaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-16
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CCollisionInfo;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  行動コンポネントクラス
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
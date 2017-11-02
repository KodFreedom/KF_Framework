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
class CollisionInfo;
class Collider;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  行動コンポネントクラス
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
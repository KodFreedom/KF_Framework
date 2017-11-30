//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorController.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerController : public ActorController
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
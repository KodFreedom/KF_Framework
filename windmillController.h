//--------------------------------------------------------------------------------
//	風車ビヘイビアコンポネント
//　WindmillController.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behavior.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class WindmillController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  変数定義
	//--------------------------------------------------------------------------------
	float		rotateSpeed;
	GameObject*	fan;
};
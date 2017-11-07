//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behavior.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class ActorController;
class CAIMode;
class CEnemyNormalMode;
class CEnemyAttackMode;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  プレイヤー行動コンポネントクラス
//--------------------------------------------------------------------------------
class EnemyController : public Behavior
{
	friend CEnemyNormalMode;
	friend CEnemyAttackMode;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  変数定義
	//--------------------------------------------------------------------------------
	GameObject*				m_pTarget;
	CAIMode*					m_pMode;
	ActorController&	m_actor;
};
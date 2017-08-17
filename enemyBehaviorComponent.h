////--------------------------------------------------------------------------------
////	エネミービヘイビアコンポネント
////　enemyBehaviorComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-07-17
////--------------------------------------------------------------------------------
//#pragma once
//
////--------------------------------------------------------------------------------
////  インクルードファイル
////--------------------------------------------------------------------------------
//#include "behaviorComponent.h"
//
////--------------------------------------------------------------------------------
////  前方宣言
////--------------------------------------------------------------------------------
//class CActorBehaviorComponent;
//class CAIMode;
//class CEnemyNormalMode;
//class CEnemyAttackMode;
//
////--------------------------------------------------------------------------------
////  クラス宣言
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  プレイヤー行動コンポネントクラス
////--------------------------------------------------------------------------------
//class CEnemyBehaviorComponent : public CBehaviorComponent
//{
//	friend CEnemyNormalMode;
//	friend CEnemyAttackMode;
//
//public:
//	//--------------------------------------------------------------------------------
//	//  関数定義
//	//--------------------------------------------------------------------------------
//	CEnemyBehaviorComponent(CGameObject* const pGameObj, CActorBehaviorComponent& actor);
//	~CEnemyBehaviorComponent() {}
//
//	bool	Init(void) override;
//	void	Uninit(void) override;
//	void	Update(void) override;
//	void	LateUpdate(void) override {}
//
//	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override;
//	void	OnCollision(CCollisionInfo& collisionInfo) override;
//
//	void	ChangeMode(CAIMode* pAIMode);
//private:
//	//--------------------------------------------------------------------------------
//	//  変数定義
//	//--------------------------------------------------------------------------------
//	CGameObject*				m_pTarget;
//	CAIMode*					m_pMode;
//	CActorBehaviorComponent&	m_actor;
//};
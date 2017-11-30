////--------------------------------------------------------------------------------
////	エネミービヘイビアコンポネント
////　EnemyController.h
////	Author : Xu Wenjie
////	Date   : 2017-07-17
////--------------------------------------------------------------------------------
//#pragma once
//
////--------------------------------------------------------------------------------
////  インクルードファイル
////--------------------------------------------------------------------------------
//#include "actorController.h"
//
////--------------------------------------------------------------------------------
////  前方宣言
////--------------------------------------------------------------------------------
//class AI;
//
////--------------------------------------------------------------------------------
////  クラス宣言
////--------------------------------------------------------------------------------
//class EnemyController : public ActorController
//{
//public:
//	//--------------------------------------------------------------------------------
//	//  関数定義
//	//--------------------------------------------------------------------------------
//	EnemyController(GameObjectActor* const owner, Rigidbody3D& rigidbody);
//	~EnemyController() {}
//
//	bool Init(void) override;
//	void Uninit(void) override;
//	void Update(void) override;
//	void LateUpdate(void) override;
//		 
//	void OnTrigger(Collider& colliderThis, Collider& collider) override;
//	void OnCollision(CollisionInfo& collisionInfo) override;
//		 
//	void Change(AI* mode);
//	auto GetTarget(void) { return target; }
//	void SetTarget(GameObject* value) { target = value; }
//	void SetAttack(const bool value) { isAttack = value; }
//	void SetMovement(const Vector3& value) { movement = value; }
//
//private:
//	//--------------------------------------------------------------------------------
//	//  変数定義
//	//--------------------------------------------------------------------------------
//	GameObject*	target;
//	AI*			currentMode;
//};
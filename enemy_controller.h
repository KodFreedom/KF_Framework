////--------------------------------------------------------------------------------
////	�G�l�~�[�r�w�C�r�A�R���|�l���g
////�@EnemyController.h
////	Author : Xu Wenjie
////	Date   : 2017-07-17
////--------------------------------------------------------------------------------
//#pragma once
//
////--------------------------------------------------------------------------------
////  �C���N���[�h�t�@�C��
////--------------------------------------------------------------------------------
//#include "actorController.h"
//
////--------------------------------------------------------------------------------
////  �O���錾
////--------------------------------------------------------------------------------
//class AI;
//
////--------------------------------------------------------------------------------
////  �N���X�錾
////--------------------------------------------------------------------------------
//class EnemyController : public ActorController
//{
//public:
//	//--------------------------------------------------------------------------------
//	//  �֐���`
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
//	//  �ϐ���`
//	//--------------------------------------------------------------------------------
//	GameObject*	target;
//	AI*			currentMode;
//};
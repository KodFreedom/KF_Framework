////--------------------------------------------------------------------------------
////	�G�l�~�[�r�w�C�r�A�R���|�l���g
////�@enemyBehaviorComponent.cpp
////	Author : Xu Wenjie
////	Date   : 2017-07-17
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  �C���N���[�h�t�@�C��
////--------------------------------------------------------------------------------
//#include "enemyNormalMode.h"
//#include "enemyBehaviorComponent.h"
//#include "gameObjectActor.h"
//#include "actorMeshComponent.h"
//#include "sphereColliderComponent.h"
//#include "colliderComponent.h"
//#include "KF_CollisionSystem.h"
//
////--------------------------------------------------------------------------------
////  �N���X
////--------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------
////  ���ʏ��
////--------------------------------------------------------------------------------
//void CEnemyNormalMode::Update(CEnemyBehaviorComponent& enemy)
//{
//	CMeshComponent* pMesh = enemy.m_pGameObj->GetMeshComponent();
//	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
//	bool bCanControl = true;
//	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) { bCanControl = false; }
//	if (!bCanControl) { return; }
//	//if (enemy.m_pAttackCollider)
//	//{
//	//	enemy.m_pGameObj->DeleteCollider(enemy.m_pAttackCollider);
//	//	enemy.m_pAttackCollider->Release();
//	//	enemy.m_pAttackCollider = nullptr;
//	//}
//
//	//�j���[�g�������[�V�����ݒ�
//	pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
//}

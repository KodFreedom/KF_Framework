//--------------------------------------------------------------------------------
//	�A�N�^�[
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectActor.h"
#include "actorBehaviorComponent.h"
#include "playerBehaviorComponent.h"
#include "enemyBehaviorComponent.h"
#include "3DRigidbodyComponent.h"
#include "actorMeshComponent.h"
#include "actorMeshDrawComponent.h"
#include "sphereColliderComponent.h"
#include "playerUIObject.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOM::PRIORITY& pri, const OBJ_TYPE& type) : CGameObject3D(pri, type)
{

}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor(GOM::PRI_3D, OT_PLAYER);

	//�R���|�l���g
	C3DRigidbodyComponent* pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;
	CPlayerBehaviorComponent* pPb = new CPlayerBehaviorComponent(pObj, pRb);
	pObj->m_listpBehavior.push_back(pPb);
	CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, 0.6f);
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);
	CActorMeshComponent* pMesh = new CActorMeshComponent(pObj);
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CActorMeshDrawComponent(pMesh, pObj);

	//�p�����[�^�[
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	//UI
	CPlayerUIObject::Create(pPb);

	return pObj;
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreateEnemy(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor(GOM::PRI_3D, OT_ENEMY);

	//�R���|�l���g
	//���W�b�h�{�f�B
	C3DRigidbodyComponent* pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;

	//�r�w�C�r�A
	CEnemyBehaviorComponent* pEb = new CEnemyBehaviorComponent(pObj, pRb);
	pObj->m_listpBehavior.push_back(pEb);

	//�R���C�_�[
	CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, 0.6f);
	pCollider->SetTag("body");
	CSphereColliderComponent* pDetector = new CSphereColliderComponent(pObj, CS::DYNAMIC, 6.0f);
	pDetector->SetTrigger(true);
	pDetector->SetTag("detector");
	pObj->AddCollider(pCollider);
	pObj->AddCollider(pDetector);

	//���b�V��
	CActorMeshComponent* pMesh = new CActorMeshComponent(pObj);
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;

	//�h���[
	pObj->m_pDraw = new CActorMeshDrawComponent(pMesh, pObj);

	//�p�����[�^�[
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}
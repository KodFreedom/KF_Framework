//--------------------------------------------------------------------------------
//	アクター
//　gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
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
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOM::PRIORITY& pri, const OBJ_TYPE& type) : CGameObject3D(pri, type)
{

}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor(GOM::PRI_3D, OT_PLAYER);

	//コンポネント
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

	//パラメーター
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//初期化
	pObj->Init();

	//UI
	CPlayerUIObject::Create(pPb);

	return pObj;
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreateEnemy(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObjectActor* pObj = new CGameObjectActor(GOM::PRI_3D, OT_ENEMY);

	//コンポネント
	//リジッドボディ
	C3DRigidbodyComponent* pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;

	//ビヘイビア
	CEnemyBehaviorComponent* pEb = new CEnemyBehaviorComponent(pObj, pRb);
	pObj->m_listpBehavior.push_back(pEb);

	//コライダー
	CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, 0.6f);
	pCollider->SetTag("body");
	CSphereColliderComponent* pDetector = new CSphereColliderComponent(pObj, CS::DYNAMIC, 6.0f);
	pDetector->SetTrigger(true);
	pDetector->SetTag("detector");
	pObj->AddCollider(pCollider);
	pObj->AddCollider(pDetector);

	//メッシュ
	CActorMeshComponent* pMesh = new CActorMeshComponent(pObj);
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;

	//ドロー
	pObj->m_pDraw = new CActorMeshDrawComponent(pMesh, pObj);

	//パラメーター
	CTransformComponent* pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}
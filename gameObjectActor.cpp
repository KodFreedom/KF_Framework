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
#include "animatorComponent.h"
#include "actorBehaviorComponent.h"
#include "playerBehaviorComponent.h"
#include "enemyBehaviorComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"
#include "AABBColliderComponent.h"
#include "playerUIObject.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor(const GOMLAYER& layer)
	: CGameObject(layer)
	, m_pAnimator(nullptr)
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
bool CGameObjectActor::Init(void)
{
	CGameObject::Init();
	m_pAnimator->Init();
	return true;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CGameObjectActor::LateUpdate(void)
{
	CGameObject::LateUpdate();
	m_pAnimator->Update();
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreatePlayer(const string &modelPath, const Vector3& vPos, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new CGameObjectActor;

	//Tag
	pObj->SetTag("Player");

	//コンポネント
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;
	pObj->m_pAnimator = new CAnimatorComponent(pObj, modelPath);
	auto pAb = new CActorBehaviorComponent(pObj, *pRb, pObj->m_pAnimator);
	auto pPb = new CPlayerBehaviorComponent(pObj, *pAb);
	pObj->m_listpBehavior.push_back(pAb);
	pObj->m_listpBehavior.push_back(pPb);
	//auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, Vector3(0.3f * vScale.X, 0.6f * vScale.Y, 0.3f * vScale.Z));
	auto pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, 0.6f);
	pCollider->SetOffset(Vector3(0.0f, 0.55f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);

	//パラメーター
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//初期化
	pObj->Init();

	//UI
	//CPlayerUIObject::Create(pPb);

	return pObj;
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CGameObjectActor* CGameObjectActor::CreateEnemy(const string &modelPath, const Vector3& vPos, const Vector3& vRot, const Vector3& vScale)
{
	auto pObj = new CGameObjectActor;

	//Tag
	pObj->SetTag("Enemy");

	//コンポネント
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->m_pRigidbody = pRb;
	pObj->m_pAnimator = new CAnimatorComponent(pObj, modelPath);
	
	//ビヘイビア
	auto pAb = new CActorBehaviorComponent(pObj, *pRb, pObj->m_pAnimator);
	auto pEb = new CEnemyBehaviorComponent(pObj, *pAb);
	pObj->m_listpBehavior.push_back(pAb);
	pObj->m_listpBehavior.push_back(pEb);
	
	//コライダー
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, Vector3(0.3f * vScale.X, 0.6f * vScale.Y, 0.3f * vScale.Z));
	pCollider->SetOffset(Vector3(0.0f, 0.6f, 0.0f));
	pCollider->SetTag("body");
	pObj->AddCollider(pCollider);
	auto pDetector = new CSphereColliderComponent(pObj, CS::DYNAMIC, 6.0f);
	pDetector->SetTrigger(true);
	pDetector->SetTag("detector");
	pObj->AddCollider(pDetector);

	//パラメーター
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CGameObjectActor::uninit(void)
{
	CGameObject::uninit();
	SAFE_RELEASE(m_pAnimator);
}
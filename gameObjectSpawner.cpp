//--------------------------------------------------------------------------------
//	ゲームオブジェクト生成関数
//　gameObjectSpawner.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "3DMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "editorControllerBehaviorComponent.h"
#include "sphereColliderComponent.h"
#include "OBBColliderComponent.h"
#include "AABBColliderComponent.h"
#include "fieldColliderComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  SkyBox生成処理
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("skyBox");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);
	pDraw->SetTexName("skybox000.jpg");
	pObj->SetDrawComponent(pDraw);

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

//--------------------------------------------------------------------------------
//  MeshField生成処理
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateField(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("RockCliff.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pCollider = new CFieldColliderComponent(pObj, 200, 200, CKFVec2(10.0f, 10.0f));
	pObj->AddCollider(pCollider);

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

//--------------------------------------------------------------------------------
//  Cube生成処理
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("cube");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("nomal_cube.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pCollider = new CAABBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//COBBColliderComponent* pCollider = new COBBColliderComponent(pObj, CS::DYNAMIC, vScale * 0.5f);
	//CSphereColliderComponent* pCollider = new CSphereColliderComponent(pObj, CS::DYNAMIC, vScale.m_fX * 0.5f);
	pObj->AddCollider(pCollider);
	auto pRb = new C3DRigidbodyComponent(pObj);
	pObj->SetRigidbodyComponent(pRb);

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

//--------------------------------------------------------------------------------
//  Cube生成処理
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	string strTexName;
	pMesh->SetMeshName(strPath, strTexName);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTexName);
	pObj->SetDrawComponent(pDraw);

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

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorController(CGameObject* pFieldEditor)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	auto pMesh = new C3DMeshComponent(pObj);
	string strTex;
	pMesh->SetMeshName("data/MODEL/target.x", strTex);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTex);
	pObj->SetDrawComponent(pDraw);
	auto pBehavior = new CEditorControllerBehaviorComponent(pObj);
	pBehavior->SetFieldEditor(pFieldEditor);
	pObj->AddBehavior(pBehavior);

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateEditorField(void)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	auto pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("RockCliff.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pBehavior = new CFieldEditorBehaviorComponent(pObj);
	pObj->AddBehavior(pBehavior);

	//パラメーター
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(CKFVec3(0.0f));
	pTrans->SetPosNext(CKFVec3(0.0f));
	pTrans->SetScale(CKFVec3(1.0f));
	pTrans->SetScaleNext(CKFVec3(1.0f));
	pTrans->RotByEuler(CKFVec3(0.0f));

	//初期化
	pObj->Init();

	return pObj;
}
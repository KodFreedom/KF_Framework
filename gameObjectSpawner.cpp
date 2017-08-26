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
#include "manager.h"
#include "meshManager.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "3DMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "modelEditorBehaviorComponent.h"
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
CGameObject* CGameObjectSpawner::CreateField(const string& strStageName)
{
	auto pObj = new CGameObject(GOM::PRI_3D);
	string strFieldName = strStageName + "Field";

	//コンポネント
	C3DMeshComponent* pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName(strFieldName);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("Grass0003_1_270.jpg");
	pObj->SetDrawComponent(pDraw);
	auto pCollider = new CFieldColliderComponent(pObj, strFieldName);
	pObj->AddCollider(pCollider);

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
	auto pMesh = new C3DMeshComponent(pObj);
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
//  Cube生成処理
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	auto pMesh = new C3DMeshComponent(pObj);
	string strTexName;
	pMesh->SetMeshName(strPath, strTexName);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTexName);
	pObj->SetDrawComponent(pDraw);

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
//	関数名：CreateModel
//  関数説明：モデルファイルからゲームオブジェクト作成
//	引数：	strFilePath：ファイルの名前 
//			vPos
//			qRot
//			vScale
//	戻り値：CGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::CreateModel(const string& strFilePath, const CKFVec3& vPos, const CKFQuaternion& qRot, const CKFVec3& vScale)
{
	if (strFilePath.find(".model") == string::npos) { return nullptr; }
	
	auto pObj = new CGameObject(GOM::PRI_3D);
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->SetRot(qRot);
	pTrans->SetRotNext(qRot);

	//Modelファイルの開く
	string strName = "data/MODEL/" + strFilePath;
	FILE *pFile;
	fopen_s(&pFile, strName.c_str(), "rb");

	//パーツ数の読込
	int nNumParentMesh;
	fread(&nNumParentMesh, sizeof(int), 1, pFile);

	for (int nCnt = 0; nCnt < nNumParentMesh; ++nCnt)
	{
		auto pObjChild = createChildMesh(pTrans, pFile);
	}

	fclose(pFile);
	return pObj;
}

//--------------------------------------------------------------------------------
//	関数名：CreateModel
//  関数説明：モデルファイルからゲームオブジェクト作成
//	引数：	strFilePath：ファイルの名前 
//			vPos
//			vRot
//			vScale
//	戻り値：CGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::createChildMesh(CTransformComponent* pParent, FILE* pFile)
{
	//ファイル名
	string strMeshName;
	int nSize = 0;
	fread(&nSize, sizeof(int), 1, pFile);
	strMeshName.resize(nSize);
	fread(&strMeshName[0], sizeof(char), nSize, pFile);

	//Offset
	CKFVec3 vPos, vRot, vScale;
	fread(&vPos, sizeof(CKFVec3), 1, pFile);
	fread(&vRot, sizeof(CKFVec3), 1, pFile);
	fread(&vScale, sizeof(CKFVec3), 1, pFile);

	auto pObj = createMesh(strMeshName, CKFVec3(0.0f), CKFVec3(0.0f), vScale);
	auto pTrans = pObj->GetTransformComponent();
	pTrans->RegisterParent(pParent, vPos, vRot);;

	//Collider
	int nNumCollider = 0;
	fread(&nNumCollider, sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumCollider; ++nCnt)
	{
		int nColType = 0;
		CKFVec3 vColPos, vColRot, vColScale;
		fread(&nColType, sizeof(int), 1, pFile);
		fread(&vColPos, sizeof(CKFVec3), 1, pFile);
		fread(&vColRot, sizeof(CKFVec3), 1, pFile);
		fread(&vColScale, sizeof(CKFVec3), 1, pFile);
		vColRot *= KF_PI / 180.0f;

		CColliderComponent* pCollider = nullptr;
		switch ((CS::COL_TYPE)nColType)
		{
		case CS::COL_SPHERE:
			pCollider = new CSphereColliderComponent(pObj, CS::STATIC, vColScale.m_fX);
			break;
		case CS::COL_AABB:
			pCollider = new CAABBColliderComponent(pObj, CS::STATIC, vColScale * 0.5f);
			break;
		case CS::COL_OBB:
			pCollider = new COBBColliderComponent(pObj, CS::STATIC, vColScale * 0.5f);
			break;
		default:
			break;
		}

		if (!pCollider) { continue; }
		pCollider->SetOffset(vColPos, vColRot);
		pObj->AddCollider(pCollider);
	}

	//Child
	int nNumChild = 0;
	fread(&nNumChild, sizeof(int), 1, pFile);
	for (int nCnt = 0; nCnt < nNumChild; ++nCnt)
	{
		auto pObjChild = createChildMesh(pTrans, pFile);
	}

	return pObj;
}

//--------------------------------------------------------------------------------
//	関数名：CreateMesh
//  関数説明：モデルファイルからゲームオブジェクト作成
//	引数：	strMeshName：ファイルの名前 
//			vPos
//			vRot
//			vScale
//	戻り値：CGameObject*
//--------------------------------------------------------------------------------
CGameObject* CGameObjectSpawner::createMesh(const string& strMeshName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	if (strMeshName.find(".mesh") == string::npos) { return nullptr; }
	auto pObj = new CGameObject(GOM::PRI_3D);

	//コンポネント
	auto pMesh = new C3DMeshComponent(pObj);
	string strTexName;
	pMesh->SetMeshName(strMeshName, strTexName);
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName(strTexName);
	pObj->SetDrawComponent(pDraw);

	//パラメーター
	auto pTrans = pObj->GetTransformComponent();
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
	pTrans->SetScale(vScale);
	pTrans->SetScaleNext(vScale);
	pTrans->RotByEuler(vRot);

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
	auto pMEBehavior = new CModelEditorBehaviorComponent(pObj);
	auto pBehavior = new CEditorControllerBehaviorComponent(pObj);
	pBehavior->SetFieldEditor(pFieldEditor);
	pBehavior->SetModelEditor(pMEBehavior);
	pObj->AddBehavior(pBehavior);
	pObj->AddBehavior(pMEBehavior);

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
	auto pBehavior = new CFieldEditorBehaviorComponent(pObj);
	pObj->AddBehavior(pBehavior);
	auto pMesh = new C3DMeshComponent(pObj);
	pMesh->SetMeshName("field");
	pObj->SetMeshComponent(pMesh);
	auto pDraw = new C3DMeshDrawComponent(pMesh, pObj);
	pDraw->SetTexName("Grass0003_1_270.jpg");
	pObj->SetDrawComponent(pDraw);

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
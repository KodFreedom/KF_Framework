//--------------------------------------------------------------------------------
//
//　gameObject3D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject3D.h"
#include "skyBoxMeshComponent.h"
#include "fieldMeshComponent.h"
#include "cubeMeshComponent.h"
#include "modelMeshComponent.h"
#include "3DMeshDrawComponent.h"
#include "modelMeshDrawComponent.h"
#include "fieldColliderComponent.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D(const GOM::PRIORITY& pri) : CGameObject(pri)
{

}

//--------------------------------------------------------------------------------
//  SkyBox生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CSkyBoxMeshComponent* pMesh = new CSkyBoxMeshComponent(pObj);
	pMesh->SetTexName(CTM::TEX_SKY);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pObj);
	pObj->m_pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;
	pObj->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  MeshField生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateField(const int& nNumBlockX, const int& nNumBlockZ, const CKFVec2& vBlockSize, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CFieldColliderComponent* pCollider = new CFieldColliderComponent(pObj, nNumBlockX, nNumBlockZ, vBlockSize);
	pObj->m_apCollider.push_back(pCollider);
	CFieldMeshComponent* pMesh = new CFieldMeshComponent(pObj, nNumBlockX, nNumBlockZ, vBlockSize);
	pMesh->SetTexName(CTM::TEX_ROAD);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pObj);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;
	pObj->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateCube(const CKFVec3& vSize, const CKFColor& cColor, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CCubeMeshComponent* pMesh = new CCubeMeshComponent(pObj, vSize, cColor);
	pMesh->SetTexName(CTM::TEX_POLYGON);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent(pObj);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;
	pObj->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  モデル生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateModel(const CMOM::MODEL_NAME &modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CModelMeshComponent* pMesh = new CModelMeshComponent(pObj);
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CModelMeshDrawComponent(pObj);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;
	pObj->RotByEuler(vRot);

	//初期化
	pObj->Init();

	return pObj;
}
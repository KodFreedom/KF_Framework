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

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D() : CGameObject(GOM::PRI_3D)
	, m_vForward(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(-1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(-1.0f, 0.0f, 0.0f))
{
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D(const GOM::PRIORITY &pri) : CGameObject(pri)
	, m_vForward(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(-1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, -1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(-1.0f, 0.0f, 0.0f))
{
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CGameObject3D::SwapParam(void)
{
	CGameObject::SwapParam();
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//  SkyBox生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateSkyBox(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CSkyBoxMeshComponent *pMesh = new CSkyBoxMeshComponent;
	pMesh->SetTexName(CTM::TEX_DEMO_SKY);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;
	pObj->m_pDraw->SetRenderState(&CDrawComponent::s_lightOffRenderState);

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  MeshField生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateField(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CFieldMeshComponent *pMesh = new CFieldMeshComponent(nNumBlockX, nNumBlockZ, vBlockSize);
	pMesh->SetTexName(CTM::TEX_DEMO_ROAD);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  Cube生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateCube(const CKFVec3 &vSize, const CKFColor &cColor, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CCubeMeshComponent *pMesh = new CCubeMeshComponent(vSize, cColor);
	pMesh->SetTexName(CTM::TEX_DEMO_POLYGON);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C3DMeshDrawComponent;

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//初期化
	pObj->Init();

	return pObj;
}

//--------------------------------------------------------------------------------
//  モデル生成処理
//--------------------------------------------------------------------------------
CGameObject3D* CGameObject3D::CreateModel(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale)
{
	CGameObject3D* pObj = new CGameObject3D;

	//コンポネント
	CModelMeshComponent *pMesh = new CModelMeshComponent();
	pMesh->SetModelName(modelName);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new CModelMeshDrawComponent;

	//パラメーター
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vRot = pObj->m_vRotNext = vRot;
	pObj->m_vScale = pObj->m_vScaleNext = vScale;

	//初期化
	pObj->Init();

	return pObj;
}
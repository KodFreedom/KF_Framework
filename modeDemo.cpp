//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "camera.h"

//gameobject
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "gameObjectActor.h"

//component
#include "fieldMeshComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
	, m_pMeshField(NULL)
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CModeDemo::~CModeDemo()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CModeDemo::Init(void)
{	
	//ライトの初期化
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//ゲームオブジェクトの初期化
	m_pMeshField = CGameObject3D::CreateField(20, 20, CKFVec2(1.0f, 1.0f), CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, 0.0f), 0.0f, CKFVec3(256.0f, 256.0f, 0.0f), CTM::TEX_DEMO_TEST);
	//CMeshCube::Create(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f), CKFColor(1.0f));
	//CGameObjectModel::Create(CKFVec3(0.0f), CKFVec3(0.0f), CMOM::MODEL_ROBOT);
	CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObjectActor::CreatePlayer(CMOM::MODEL_PLAYER, CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::Uninit();

	//ライトの破棄
	GetManager()->GetLightManager()->ReleaseAll();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();
}

float CModeDemo::GetHeight(const CKFVec3 &vPos)
{
	CFieldMeshComponent* pField = (CFieldMeshComponent*)m_pMeshField->GetMeshComponent();
	return pField->GetHeight(vPos);
}
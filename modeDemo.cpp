//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "modeResult.h"
#include "actionGameCamera.h"
#include "fade.h"

//gameobject
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
	, m_bEndMode(false)
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
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//ゲームオブジェクトの初期化
	CGameObject3D::CreateField(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		CKFVec3 vPos = CKFMath::GetRandomVec3(CKFVec3(-10.0f, 4.0f, -10.0f), CKFVec3(10.0f, 5.0f, 10.0f));
		CKFVec3 vRot = CKFMath::GetRandomVec3(CKFVec3(-KF_PI, -KF_PI, -KF_PI), CKFVec3(KF_PI, KF_PI, KF_PI));
		CGameObject3D::CreateCube(vPos, vRot, CKFVec3(1.2f));
	}
	
	CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject* pPlayer = CGameObjectActor::CreatePlayer(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject* pEnemy = CGameObjectActor::CreateEnemy(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 10.0f), CKFVec3(0.0f, KF_PI, 0.0f), CKFVec3(1.0f));

	m_pCamera->SetTarget(pPlayer);

	GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
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

	//BGMの停止
	GetManager()->GetSoundManager()->StopAll();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	if (m_bEndMode || GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	{
		GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();
}
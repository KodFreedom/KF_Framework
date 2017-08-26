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
#include "stageSpawner.h"
#include "gameObjectSpawner.h"
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
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//ゲームオブジェクトの初期化
	CGameObjectSpawner::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CStageSpawner::LoadStage("demo");
	CGameObject* pPlayer = CGameObjectActor::CreatePlayer("data/MODEL/motionPlayer.txt", CKFVec3(0.0f, 10.0f, 0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//CGameObject* pEnemy = CGameObjectActor::CreateEnemy(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 10.0f), CKFVec3(0.0f, KF_PI, 0.0f), CKFVec3(1.0f));

	m_pCamera->SetTarget(pPlayer);

	//CMain::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::Uninit();

	//ライトの破棄
	CMain::GetManager()->GetLightManager()->ReleaseAll();

	//BGMの停止
	CMain::GetManager()->GetSoundManager()->StopAll();
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

	if (m_bEndMode || CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_START))
	{
		CMain::GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}
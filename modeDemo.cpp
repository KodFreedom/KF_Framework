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

#ifdef _DEBUG
#include "debugManager.h"
#endif // _DEBUG

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
	Main::GetManager()->GetLightManager()->CreateDirectionalLight(Vector3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//ゲームオブジェクトの初期化
	CGameObjectSpawner::CreateSkyBox(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	CStageSpawner::LoadStage("demo");
	//CGameObjectSpawner::CreateCube(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	auto pPlayer = CGameObjectActor::CreatePlayer("data/MODEL/motionPlayer.txt", Vector3(119.7f, 10.0f, -121.2f), Vector3(0.0f), Vector3(1.0f));
	pPlayer->SetName("Player");

#ifdef _DEBUG
	Main::GetManager()->GetDebugManager()->SetPlayer(pPlayer);
#endif // _DEBUG
	//auto pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-50.0, 30.0f, -7.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy00");
	//pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-12.0, 45.0f, -46.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy01");
	//pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(117.0f, 80.0f, 1.6f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy02");
	CGameObjectSpawner::CreateGoal(Vector3(112.0f, 16.0f, 51.0f));
	m_pCamera->SetTarget(pPlayer);

	//Main::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
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

	if (Main::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_START))
	{
		Main::GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeDemo::uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::uninit();

	//ライトの破棄
	Main::GetManager()->GetLightManager()->ReleaseAll();

	//BGMの停止
	Main::GetManager()->GetSoundManager()->StopAll();
}
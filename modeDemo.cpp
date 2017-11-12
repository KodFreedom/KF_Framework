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
#include "input.h"
#include "soundManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "modeResult.h"
#include "thirdPersonCamera.h"
#include "fadeSystem.h"

//gameobject
#include "stageSpawner.h"
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ModeDemo::ModeDemo() : Mode("Demo")
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
ModeDemo::~ModeDemo()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ModeDemo::Init(void)
{	
	LightManager::Instance()->CreateLight(LightType::LT_Directional);

	//カメラの初期化
	auto camera = new ThirdPersionCamera;
	camera->Init();

	//ゲームオブジェクトの初期化
	GameObjectSpawner::CreateSkyBox(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	StageSpawner::LoadStage("demo");
	//GameObjectSpawner::CreateCube(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	auto player = GameObjectSpawner::CreatePlayer("data/MODEL/motionPlayer.txt", Vector3(119.7f, 10.0f, -121.2f), Vector3(0.0f), Vector3(1.0f));
	player->SetName("Player");

#ifdef _DEBUG
	DebugObserver::Instance()->SetPlayer(player);
#endif // _DEBUG
	//auto pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-50.0, 30.0f, -7.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy00");
	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-12.0, 45.0f, -46.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy01");
	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(117.0f, 80.0f, 1.6f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy02");
	GameObjectSpawner::CreateGoal(Vector3(112.0f, 16.0f, 51.0f));
	camera->SetFollowTarget(player);

	//Main::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeDemo::Update(void)
{
	Mode::Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void ModeDemo::LateUpdate(void)
{
	Mode::LateUpdate();

	if (Input::Instance()->GetKeyTrigger(Key::Start))
	{
		FadeSystem::Instance()->FadeTo(new ModeResult);
	}
}
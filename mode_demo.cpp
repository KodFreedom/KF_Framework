//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_demo.h"
#include "main_system.h"
#include "light_manager.h"
#include "input.h"
#include "sound_manager.h"
#include "mode_result.h"
#include "third_person_camera.h"
#include "fade_system.h"

//gameobject
#include "stage_spawner.h"
#include "game_object_spawner.h"

#ifdef _DEBUG
#include "debug_observer.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ModeDemo::ModeDemo() : Mode(L"Demo")
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
	auto main_system = MainSystem::Instance();
	main_system->GetLightManager()->CreateDirectionLight(Vector3(-1.0f, -1.0f, 1.0f).Normalized());

	//カメラの初期化
	auto camera = MY_NEW ThirdPersionCamera;
	camera->Init();

	//MotionManager::Instance()->CreateMotionFileBy("data/MODEL/motionPlayer.txt");
	//MaterialManager::Instance()->CreateMaterialFileBy("sky", "skybox000.jpg");
	//MaterialManager::Instance()->CreateMaterialFileBy("demoField", "demoField.jpg");
	//MaterialManager::Instance()->CreateMaterialFileBy("cube", "nomal_cube.jpg");
	//MaterialManager::Instance()->CreateMaterialFileBy("editorField", "editorField.jpg");
//	//ゲームオブジェクトの初期化
	GameObjectSpawner::CreateSkyBox(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	StageSpawner::LoadStage(L"demo");
//	//GameObjectSpawner::CreateCube(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
//	auto player = GameObjectSpawner::CreatePlayer("data/MODEL/motionPlayer.txt", Vector3(119.7f, 10.0f, -121.2f), Vector3(0.0f), Vector3(1.0f));
//	player->SetName("Player");
//
//#ifdef _DEBUG
//	DebugObserver::Instance()->SetPlayer(player);
//#endif // _DEBUG
//	//auto pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-50.0, 30.0f, -7.0f), Vector3(0.0f), Vector3(1.0f));
//	//pEnemy->SetName("Enemy00");
//	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-12.0, 45.0f, -46.0f), Vector3(0.0f), Vector3(1.0f));
//	//pEnemy->SetName("Enemy01");
//	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(117.0f, 80.0f, 1.6f), Vector3(0.0f), Vector3(1.0f));
//	//pEnemy->SetName("Enemy02");
//	GameObjectSpawner::CreateGoal(Vector3(112.0f, 16.0f, 51.0f));
//	camera->SetFollowTarget(player);
//
//	//Main::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
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

	auto main_system = MainSystem::Instance();
	if (main_system->GetInput()->GetKeyTrigger(Key::kStart))
	{
		main_system->GetFadeSystem()->FadeTo(MY_NEW ModeResult);
	}
}
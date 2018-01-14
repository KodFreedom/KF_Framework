//--------------------------------------------------------------------------------
//　stage_spawner.h
//	ステージ生成処理
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include <cereal/archives/binary.hpp>
using namespace cereal;
#include "kf_math.h"
using namespace kodfreedom;
#include "stage_spawner.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "game_object_actor.h"
#include "windmill_controller.h"
#include "main_system.h"
#include "shadow_map_system.h"
#include "third_person_camera.h"
#include "light.h"

#ifdef _DEBUG
#include "debug_observer.h"
#endif // _DEBUG
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
void StageSpawner::LoadStage(const String& stage_name)
{
    auto main_system = MainSystem::Instance();

    //カメラの作成
    auto camera = MY_NEW ThirdPersionCamera;
    camera->Init();

    //ライトの作成
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

    //空の作成
    GameObjectSpawner::CreateSkyBox(Vector3::kZero, Vector3::kZero, Vector3::kOne);

    //地面の作成
	GameObjectSpawner::CreateField(stage_name);

	//ステージの読込
	String path = L"data/stage/" + stage_name + L"Stage.stage";
	ifstream file(path, ios::binary);
	if (!file.is_open())
	{
		assert(file.is_open());
		return;
	}
	BinaryInputArchive archive(file);

	//Model数の読込
	size_t model_number;
	archive.loadBinary(&model_number, sizeof model_number);
	for (size_t count = 0; count < model_number; ++count)
	{
		//ファイル名読込
		size_t name_size;
		archive.loadBinary(&name_size, sizeof(name_size));
		string name;
		name.resize(name_size);
		archive.loadBinary(&name[0], name_size);
		String model_name = String(name.begin(), name.end());

		//作ったモデル数の読込
		size_t created_model_number;
		archive.loadBinary(&created_model_number, sizeof(created_model_number));

		//位置回転スケールの読込
		for (size_t count_model = 0; count_model < created_model_number; ++count_model)
		{
			Vector3 position;
			archive.loadBinary(&position, sizeof(position));
			Quaternion rotation;
			archive.loadBinary(&rotation, sizeof(rotation));
			Vector3 scale;
			archive.loadBinary(&scale, sizeof(scale));

			// 作成
			auto model = GameObjectSpawner::CreateModel(model_name, position, rotation, scale);
			//if (modelName == "Medieval_Windmill.model")
			//{
			//	auto behavior = MY_NEW WindmillController(gameObject);
			//	behavior->Init();
			//	gameObject->AddBehavior(behavior);
			//}
		}
	}
	file.close();

    // Playerの作成
    path = L"data/stage/" + stage_name + L"Player.player";
    file = ifstream(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive player_archive(file);
    Vector3 position;
    player_archive.loadBinary(&position, sizeof(position));
    auto player = GameObjectSpawner::CreatePlayer(L"mutant", position, Vector3::kZero, Vector3::kOne);
    player->SetName(L"Player");
    file.close();
    
    //システムにプレイヤーを設定する
    camera->SetFollowTarget(player);
    main_system->GetShadowMapSystem()->SetTarget(player->GetTransform());
#ifdef _DEBUG
    main_system->GetDebugObserver()->SetPlayer(player);
#endif // _DEBUG

    // Enemy
    GameObjectSpawner::CreateEnemy(L"derrick", Vector3(10.0f, 10.0f, 10.0f), Vector3::kZero, Vector3::kOne);
}
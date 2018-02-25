//--------------------------------------------------------------------------------
//　stage_spawner.h
//  ステージ生成処理
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "../../library/cereal/archives/binary.hpp"
using namespace cereal;
#include "../kf_utility/kf_math.h"
using namespace kodfreedom;
#include "stage_spawner.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "game_object_actor.h"
#include "../component/behavior/other/windmill_controller.h"
#include "../main_system.h"
#include "../camera/third_person_camera.h"
#include "../light/light.h"
#include "../component/behavior/actor/enemy_controller.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
void StageSpawner::LoadStage(const String& stage_name)
{
    auto& main_system = MainSystem::Instance();

    //カメラの作成
    auto camera = MY_NEW ThirdPersionCamera;
    camera->Init();

    //ライトの作成
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

    //空の作成
    GameObjectSpawner::CreateSkyBox(Vector3::kZero, Vector3::kZero, Vector3::kOne);

    //地面の作成
    GameObjectSpawner::CreateField(stage_name);

    // 環境の読込
    LoadEnvironment(stage_name);

    // Playerの読込
    LoadPlayer(stage_name);

    // Enemy
    LoadEnemy(stage_name);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  環境の読込
//--------------------------------------------------------------------------------
void StageSpawner::LoadEnvironment(const String& stage_name)
{
    //ステージの読込
    String& path = L"data/stage/" + stage_name + L".stage";
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
            if (model_name == L"Medieval_Windmill.model")
            {
                auto behavior = MY_NEW WindmillController(*model);
                behavior->Init();
                model->AddBehavior(behavior);
            }
        }
    }
    file.close();
}

//--------------------------------------------------------------------------------
//  プレイヤーの読込
//--------------------------------------------------------------------------------
void StageSpawner::LoadPlayer(const String& stage_name)
{
    String& path = L"data/stage/" + stage_name + L".player";
    ifstream file = ifstream(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive player_archive(file);
    Vector3 position;
    player_archive.loadBinary(&position, sizeof(position));
    auto player = GameObjectSpawner::CreatePlayer(L"knight", position, Vector3::kZero, Vector3::kOne);;
    file.close();
}

//--------------------------------------------------------------------------------
//  エネミーの読込
//--------------------------------------------------------------------------------
void StageSpawner::LoadEnemy(const String& stage_name)
{
    String& path = L"data/stage/" + stage_name + L".enemy";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive archive(file);

    // enemy数の読込
    size_t enemy_number;
    archive.loadBinary(&enemy_number, sizeof enemy_number);

    // 情報の読込
    for (size_t count = 0; count < enemy_number; ++count)
    {
        // 位置
        Vector3 position;
        archive.loadBinary(&position, sizeof(position));

        auto enemy = GameObjectSpawner::CreateEnemy(L"derrick", position, Vector3::kZero, Vector3::kOne);
        auto behavior = enemy->GetBehaviorBy(L"EnemyController");
        assert(behavior);
        auto enemy_controller = static_cast<EnemyController*>(behavior);

        // 警戒範囲
        float warning_range;
        archive.loadBinary(&warning_range, sizeof(warning_range));
        enemy_controller->SetWarningRange(warning_range);

        // 巡回範囲
        float patrol_range;
        archive.loadBinary(&patrol_range, sizeof(patrol_range));
        enemy_controller->SetPatrolRange(patrol_range);

        // パラメーター
        ActorParameter parameter;
        archive.loadBinary(&parameter, sizeof(parameter));
        enemy_controller->SetParameter(parameter);
    }

    file.close();

    // UI
    GameObjectSpawner::CreateEnemyUiController();
}
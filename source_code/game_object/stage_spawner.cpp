//--------------------------------------------------------------------------------
//�@stage_spawner.h
//  �X�e�[�W��������
//  Author : �����^(KodFreedom)
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

    //�J�����̍쐬
    auto camera = MY_NEW ThirdPersionCamera;
    camera->Init();

    //���C�g�̍쐬
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

    //��̍쐬
    GameObjectSpawner::CreateSkyBox(Vector3::kZero, Vector3::kZero, Vector3::kOne);

    //�n�ʂ̍쐬
    GameObjectSpawner::CreateField(stage_name);

    // ���̓Ǎ�
    LoadEnvironment(stage_name);

    // Player�̓Ǎ�
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
//  ���̓Ǎ�
//--------------------------------------------------------------------------------
void StageSpawner::LoadEnvironment(const String& stage_name)
{
    //�X�e�[�W�̓Ǎ�
    String& path = L"data/stage/" + stage_name + L".stage";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive archive(file);

    //Model���̓Ǎ�
    size_t model_number;
    archive.loadBinary(&model_number, sizeof model_number);
    for (size_t count = 0; count < model_number; ++count)
    {
        //�t�@�C�����Ǎ�
        size_t name_size;
        archive.loadBinary(&name_size, sizeof(name_size));
        string name;
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        String model_name = String(name.begin(), name.end());

        //��������f�����̓Ǎ�
        size_t created_model_number;
        archive.loadBinary(&created_model_number, sizeof(created_model_number));

        //�ʒu��]�X�P�[���̓Ǎ�
        for (size_t count_model = 0; count_model < created_model_number; ++count_model)
        {
            Vector3 position;
            archive.loadBinary(&position, sizeof(position));
            Quaternion rotation;
            archive.loadBinary(&rotation, sizeof(rotation));
            Vector3 scale;
            archive.loadBinary(&scale, sizeof(scale));

            // �쐬
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
//  �v���C���[�̓Ǎ�
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
//  �G�l�~�[�̓Ǎ�
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

    // enemy���̓Ǎ�
    size_t enemy_number;
    archive.loadBinary(&enemy_number, sizeof enemy_number);

    // ���̓Ǎ�
    for (size_t count = 0; count < enemy_number; ++count)
    {
        // �ʒu
        Vector3 position;
        archive.loadBinary(&position, sizeof(position));

        auto enemy = GameObjectSpawner::CreateEnemy(L"derrick", position, Vector3::kZero, Vector3::kOne);
        auto behavior = enemy->GetBehaviorBy(L"EnemyController");
        assert(behavior);
        auto enemy_controller = static_cast<EnemyController*>(behavior);

        // �x���͈�
        float warning_range;
        archive.loadBinary(&warning_range, sizeof(warning_range));
        enemy_controller->SetWarningRange(warning_range);

        // ����͈�
        float patrol_range;
        archive.loadBinary(&patrol_range, sizeof(patrol_range));
        enemy_controller->SetPatrolRange(patrol_range);

        // �p�����[�^�[
        ActorParameter parameter;
        archive.loadBinary(&parameter, sizeof(parameter));
        enemy_controller->SetParameter(parameter);
    }

    file.close();

    // UI
    GameObjectSpawner::CreateEnemyUiController();
}
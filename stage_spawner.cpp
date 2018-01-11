//--------------------------------------------------------------------------------
//�@stage_spawner.h
//	�X�e�[�W��������
//	Author : �����^(KodFreedom)
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

    //�J�����̍쐬
    auto camera = MY_NEW ThirdPersionCamera;
    camera->Init();

    //���C�g�̍쐬
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

    //��̍쐬
    GameObjectSpawner::CreateSkyBox(Vector3::kZero, Vector3::kZero, Vector3::kOne);

    //�n�ʂ̍쐬
	GameObjectSpawner::CreateField(stage_name);

	//�X�e�[�W�̓Ǎ�
	String path = L"data/stage/" + stage_name + L"Stage.stage";
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
			//if (modelName == "Medieval_Windmill.model")
			//{
			//	auto behavior = MY_NEW WindmillController(gameObject);
			//	behavior->Init();
			//	gameObject->AddBehavior(behavior);
			//}
		}
	}
	file.close();

    // Player�̍쐬
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
    
    //�V�X�e���Ƀv���C���[��ݒ肷��
    camera->SetFollowTarget(player);
    main_system->GetShadowMapSystem()->SetTarget(player->GetTransform());
#ifdef _DEBUG
    main_system->GetDebugObserver()->SetPlayer(player);
#endif // _DEBUG

    // Enemy
    GameObjectSpawner::CreateEnemy(L"derrick", Vector3(10.0f, 10.0f, 10.0f), Vector3::kZero, Vector3::kOne);
}
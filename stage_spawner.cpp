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
#include "windmill_controller.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
void StageSpawner::LoadStage(const String& stage_name)
{
	GameObjectSpawner::CreateField(stage_name);

	//�t�B�[���h�̓Ǎ�
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
}
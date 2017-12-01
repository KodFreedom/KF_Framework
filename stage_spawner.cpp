//--------------------------------------------------------------------------------
//　stage_spawner.h
//	ステージ生成処理
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
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

	////フィールドの保存
	//string filePath = "data/STAGE/" + stageName + "Stage" + ".stage";
	//FILE *filePointer;

	////file open
	//fopen_s(&filePointer, filePath.c_str(), "rb");

	//if (!filePointer)
	//{
	//	MessageBox(NULL, "StageSpawner : LoadStage ERROR!! ファイルが見つからない!!", "エラー", MB_OK | MB_ICONWARNING);
	//	return;
	//}

	//int modelTypeNumber = 0;
	//fread(&modelTypeNumber, sizeof(int), 1, filePointer);

	//for (int count = 0; count < modelTypeNumber; ++count)
	//{
	//	//ファイル名読込
	//	int size = 0;
	//	fread(&size, sizeof(int), 1, filePointer);
	//	string modelName;
	//	modelName.resize(size);
	//	fread(&modelName[0], sizeof(char), size, filePointer);
	//	modelName += ".model";
	//	
	//	//モデル数の読込
	//	int number = 0;
	//	fread(&number, sizeof(int), 1, filePointer);

	//	//位置回転の読込
	//	for (int countModel = 0; countModel < number; ++countModel)
	//	{
	//		Vector3 position;
	//		fread(&position, sizeof(Vector3), 1, filePointer);
	//		Quaternion rotation;
	//		fread(&rotation, sizeof(Quaternion), 1, filePointer);
	//		auto gameObject = GameObjectSpawner::CreateModel(modelName, position, rotation, Vector3::One);
	//		if (modelName == "Medieval_Windmill.model")
	//		{
	//			auto behavior = new WindmillController(gameObject);
	//			behavior->Init();
	//			gameObject->AddBehavior(behavior);
	//		}
	//	}
	//}

	//fclose(filePointer);
}
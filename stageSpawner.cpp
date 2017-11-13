//--------------------------------------------------------------------------------
//	ステージ生成関数
//　stageSpawner.h
//	Author : Xu Wenjie
//	Date   : 2017-08-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshManager.h"
#include "stageSpawner.h"
#include "gameObjectSpawner.h"
#include "gameObject.h"
#include "windmillController.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
void StageSpawner::LoadStage(const string& stageName)
{
	GameObjectSpawner::CreateField(stageName);

	//フィールドの保存
	string filePath = "data/STAGE/" + stageName + "Stage" + ".stage";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "rb");

	if (!filePointer)
	{
		MessageBox(NULL, "StageSpawner : LoadStage ERROR!! ファイルが見つからない!!", "エラー", MB_OK | MB_ICONWARNING);
		return;
	}

	int modelTypeNumber = 0;
	fread(&modelTypeNumber, sizeof(int), 1, filePointer);

	for (int count = 0; count < modelTypeNumber; ++count)
	{
		//ファイル名読込
		int size = 0;
		fread(&size, sizeof(int), 1, filePointer);
		string modelName;
		modelName.resize(size);
		fread(&modelName[0], sizeof(char), size, filePointer);
		modelName += ".model";
		
		//モデル数の読込
		int number = 0;
		fread(&number, sizeof(int), 1, filePointer);

		//位置回転の読込
		for (int countModel = 0; countModel < number; ++countModel)
		{
			Vector3 position;
			fread(&position, sizeof(Vector3), 1, filePointer);
			Quaternion rotation;
			fread(&rotation, sizeof(Quaternion), 1, filePointer);
			auto gameObject = GameObjectSpawner::CreateModel(modelName, position, rotation, Vector3::One);
			if (modelName == "Medieval_Windmill.model")
			{
				auto behavior = new WindmillController(gameObject);
				behavior->Init();
				gameObject->AddBehavior(behavior);
			}
		}
	}

	fclose(filePointer);
}
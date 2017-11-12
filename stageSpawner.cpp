//--------------------------------------------------------------------------------
//	�X�e�[�W�����֐�
//�@stageSpawner.h
//	Author : Xu Wenjie
//	Date   : 2017-08-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
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
void StageSpawner::LoadStage(const string& strStageName)
{
	GameObjectSpawner::CreateField(strStageName);

	//�t�B�[���h�̕ۑ�
	string strName = "data/STAGE/" + strStageName + "Stage" + ".stage";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "rb");

	if (!filePointer)
	{
		MessageBox(NULL, "StageSpawner : LoadStage ERROR!! �t�@�C����������Ȃ�!!", "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}

	int nNumModelType = 0;
	fread(&nNumModelType, sizeof(int), 1, filePointer);

	for (int count = 0; count < nNumModelType; ++count)
	{
		//�t�@�C�����Ǎ�
		int size = 0;
		fread(&size, sizeof(int), 1, filePointer);
		string strModelName;
		strModelName.resize(size);
		fread(&strModelName[0], sizeof(char), size, filePointer);
		strModelName += ".model";
		
		//���f�����̓Ǎ�
		int number = 0;
		fread(&number, sizeof(int), 1, filePointer);

		//�ʒu��]�̓Ǎ�
		for (int countModel = 0; countModel < number; ++countModel)
		{
			Vector3 position;
			fread(&position, sizeof(Vector3), 1, filePointer);
			Quaternion rotation;
			fread(&rotation, sizeof(Quaternion), 1, filePointer);
			auto gameObject = GameObjectSpawner::CreateModel(strModelName, position, rotation, Vector3(1.0f));
			if (strModelName == "Medieval_Windmill.model")
			{
				auto behavior = new WindmillController(gameObject);
				behavior->Init();
				gameObject->AddBehavior(behavior);
			}
		}
	}

	fclose(filePointer);
}
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
#include "windmillBehaviorComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
void CStageSpawner::LoadStage(const string& strStageName)
{
	GameObjectSpawner::CreateField(strStageName);

	//フィールドの保存
	string strName = "data/STAGE/" + strStageName + "Stage" + ".stage";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "rb");

	if (!filePointer)
	{
		MessageBox(NULL, "CStageSpawner : LoadStage ERROR!! ファイルが見つからない!!", "エラー", MB_OK | MB_ICONWARNING);
		return;
	}

	int nNumModelType = 0;
	fread(&nNumModelType, sizeof(int), 1, filePointer);

	for (int count = 0; count < nNumModelType; ++count)
	{
		//ファイル名読込
		int nSize = 0;
		fread(&nSize, sizeof(int), 1, filePointer);
		string strModelName;
		strModelName.resize(nSize);
		fread(&strModelName[0], sizeof(char), nSize, filePointer);
		strModelName += ".model";

		//モデル数の読込
		int nNum = 0;
		fread(&nNum, sizeof(int), 1, filePointer);

		//位置回転の読込
		for (int countModel = 0; countModel < nNum; ++countModel)
		{
			Vector3 Position;
			fread(&Position, sizeof(Vector3), 1, filePointer);
			Quaternion qRot;
			fread(&qRot, sizeof(Quaternion), 1, filePointer);
			auto pObj = GameObjectSpawner::CreateModel(strModelName, Position, qRot, Vector3(1.0f));
			if (strModelName == "Medieval_Windmill.model")
			{
				auto pBehavior = new CWindmillBehaviorComponent(pObj);
				pBehavior->Init();
				pObj->AddBehavior(pBehavior);
			}
		}
	}

	fclose(filePointer);
}
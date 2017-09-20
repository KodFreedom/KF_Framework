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

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
void CStageSpawner::LoadStage(const string& strStageName)
{
	CGameObjectSpawner::CreateField(strStageName);

	//�t�B�[���h�̕ۑ�
	string strName = "data/STAGE/" + strStageName + "Stage" + ".stage";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "rb");

	if (!pFile)
	{
		MessageBox(NULL, "CStageSpawner : LoadStage ERROR!! �t�@�C����������Ȃ�!!", "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}

	int nNumModelType = 0;
	fread(&nNumModelType, sizeof(int), 1, pFile);

	for (int nCnt = 0; nCnt < nNumModelType; ++nCnt)
	{
		//�t�@�C�����Ǎ�
		int nSize = 0;
		fread(&nSize, sizeof(int), 1, pFile);
		string strModelName;
		strModelName.resize(nSize);
		fread(&strModelName[0], sizeof(char), nSize, pFile);
		strModelName += ".model";

		//���f�����̓Ǎ�
		int nNum = 0;
		fread(&nNum, sizeof(int), 1, pFile);

		//�ʒu��]�̓Ǎ�
		for (int nCntModel = 0; nCntModel < nNum; ++nCntModel)
		{
			CKFVec3 vPos;
			fread(&vPos, sizeof(CKFVec3), 1, pFile);
			CKFQuaternion qRot;
			fread(&qRot, sizeof(CKFQuaternion), 1, pFile);
			CGameObjectSpawner::CreateModel(strModelName, vPos, qRot, CKFVec3(1.0f));
		}
	}

	fclose(pFile);
}
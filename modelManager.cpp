//--------------------------------------------------------------------------------
//
//　modelManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "modelManager.h"
#include "model.h"
#include "modelX.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
CModelManager::MODEL_INFO CModelManager::m_apModelInfo[MODEL_MAX] =
{
	{ XFILE, NULL },									//NULL
	{ XFILE, "data/MODEL/cube.x" },						//cube					//robot
	{ XFILE_MOTION, "data/MODEL/motionPlayer.txt" },	//player
	//{ XFILE_MOTION, "data/MODEL/motionEnemy.txt" },		//enemy
};

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelManager::CModelManager()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CModelManager::Init(void)
{
	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  モデルの読み込み
//--------------------------------------------------------------------------------
void CModelManager::LoadAll(void)
{
	Load(MODEL_NONE, MODEL_MAX);
}

//--------------------------------------------------------------------------------
//  モデルの破棄
//--------------------------------------------------------------------------------
void CModelManager::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < (int)m_vectorModel.size(); nCnt++)
	{
		if (m_vectorModel[nCnt] != NULL)
		{
			m_vectorModel[nCnt]->Uninit();
			delete m_vectorModel[nCnt];
			m_vectorModel[nCnt] = NULL;
		}
	}

	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  モデルの取得
//--------------------------------------------------------------------------------
CModel *CModelManager::GetModel(const MODEL_NAME &modelName)
{
	if (modelName <= MODEL_NONE || modelName >= MODEL_MAX) { return NULL; }

	return m_vectorModel[(int)modelName];
}

//--------------------------------------------------------------------------------
//  モデルの読み込み
//--------------------------------------------------------------------------------
void CModelManager::Load(const MODEL_NAME &modelBegin, const MODEL_NAME &modelEnd)
{
	for (int nCnt = modelBegin; nCnt < modelEnd; nCnt++)
	{
		CModel *pModel = NULL;

		if (m_apModelInfo[nCnt].path != NULL)
		{
			switch (m_apModelInfo[nCnt].type)
			{
			case XFILE:
				pModel = CModelX::Create(m_apModelInfo[nCnt].path);
				break;
			case XFILE_MOTION:
				pModel = CModelActorX::Create(m_apModelInfo[nCnt].path);
				break;
			default:
				break;
			}
		}

		m_vectorModel.push_back(pModel);
	}
}
//--------------------------------------------------------------------------------
//
//�@modelManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "modelManager.h"
#include "model.h"
#include "modelX.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
CModelManager::MODEL_INFO CModelManager::m_apModelInfo[MODEL_MAX] =
{
	{ XFILE, NULL },									//NULL
	{ XFILE, "data/MODEL/cube.x" },						//cube					//robot
	{ XFILE_MOTION, "data/MODEL/motionPlayer.txt" },	//player
	//{ XFILE_MOTION, "data/MODEL/motionEnemy.txt" },		//enemy
};

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelManager::CModelManager()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CModelManager::Init(void)
{
	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  ���f���̓ǂݍ���
//--------------------------------------------------------------------------------
void CModelManager::LoadAll(void)
{
	Load(MODEL_NONE, MODEL_MAX);
}

//--------------------------------------------------------------------------------
//  ���f���̔j��
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
//  ���f���̎擾
//--------------------------------------------------------------------------------
CModel *CModelManager::GetModel(const MODEL_NAME &modelName)
{
	if (modelName <= MODEL_NONE || modelName >= MODEL_MAX) { return NULL; }

	return m_vectorModel[(int)modelName];
}

//--------------------------------------------------------------------------------
//  ���f���̓ǂݍ���
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
//--------------------------------------------------------------------------------
//
//�@gameObjManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObjectManager.h"

//gameObject head files
#include "gameObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "meshCube.h"
#include "meshField.h"
#include "skyBox.h"
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectManager::CGameObjectManager()
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CGameObjectManager::Init(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CGameObjectManager::Uninit(void)
{
	ReleaseAll();
}

//--------------------------------------------------------------------------------
//  �����[�X����
//--------------------------------------------------------------------------------
void CGameObjectManager::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{

			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Uninit();
				delete m_avectorGameObj[nCntPri][nCnt];
				m_avectorGameObj[nCntPri][nCnt] = NULL;
			}
		}
		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectManager::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Update();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObjectManager::LateUpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->LateUpdate();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectManager::DrawAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Draw();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̊m��
//--------------------------------------------------------------------------------
int CGameObjectManager::SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	//�z��̊Ԃɋ󂫂�����ꍇ
	for (int nCnt = 0; nCnt < (int)m_avectorGameObj[pri].size(); nCnt++)
	{
		if (m_avectorGameObj[pri][nCnt] == NULL)
		{
			m_avectorGameObj[pri][nCnt] = pGameObj;
			return nCnt;
		}
	}

	//�z��̊Ԃɋ󂫂��Ȃ��ꍇ
	m_avectorGameObj[pri].push_back(pGameObj);
	return (int)m_avectorGameObj[pri].size();
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̔j��
//--------------------------------------------------------------------------------
void CGameObjectManager::ReleaseGameObj(const PRIORITY &pri, const int &nID)
{
	PRIORITY priCopy = pri;
	int nIDCopy = nID;

	if (m_avectorGameObj[priCopy][nIDCopy] != NULL)
	{
		m_avectorGameObj[priCopy][nIDCopy]->Uninit();
		delete m_avectorGameObj[priCopy][nIDCopy];
		m_avectorGameObj[priCopy][nIDCopy] = NULL;
	}
}
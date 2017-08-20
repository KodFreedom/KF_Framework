//--------------------------------------------------------------------------------
//
//�@gameObjManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObjectManager.h"

//gameObject head files
#include "gameObject.h"
#include "gameObjectActor.h"

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
		m_alistGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CGameObjectManager::Init(void)
{
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
		for (auto itr = m_alistGameObj[nCntPri].begin(); itr != m_alistGameObj[nCntPri].end();)
		{
			(*itr)->Uninit();
			delete (*itr);
			itr = m_alistGameObj[nCntPri].erase(itr);
		}
		//for (int nCnt = 0; nCnt < (int)m_alistGameObj[nCntPri].size(); nCnt++)
		//{
		//	if (m_alistGameObj[nCntPri][nCnt] != NULL)
		//	{
		//		m_alistGameObj[nCntPri][nCnt]->Uninit();
		//		delete m_alistGameObj[nCntPri][nCnt];
		//		m_alistGameObj[nCntPri][nCnt] = NULL;
		//	}
		//}
		//m_alistGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectManager::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (auto itr = m_alistGameObj[nCntPri].begin(); itr != m_alistGameObj[nCntPri].end(); ++itr)
		{
			(*itr)->Update();
		}
		//for (int nCnt = 0; nCnt < (int)m_alistGameObj[nCntPri].size(); nCnt++)
		//{
		//	if (m_alistGameObj[nCntPri][nCnt] != NULL)
		//	{
		//		m_alistGameObj[nCntPri][nCnt]->Update();
		//	}
		//}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObjectManager::LateUpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (auto itr = m_alistGameObj[nCntPri].begin(); itr != m_alistGameObj[nCntPri].end(); ++itr)
		{
			(*itr)->LateUpdate();
		}
		//for (int nCnt = 0; nCnt < (int)m_alistGameObj[nCntPri].size(); nCnt++)
		//{
		//	if (m_alistGameObj[nCntPri][nCnt] != NULL)
		//	{
		//		m_alistGameObj[nCntPri][nCnt]->LateUpdate();
		//	}
		//}
	}


	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (auto itr = m_alistGameObj[nCntPri].begin(); itr != m_alistGameObj[nCntPri].end();)
		{
			if (!(*itr)->m_bAlive)
			{//�����ĂȂ��I�u�W�F�N�g���폜
				(*itr)->Uninit();
				delete (*itr);
				itr = m_alistGameObj[nCntPri].erase(itr);
			}
			else
			{
				++itr;
			}
		}
		//for (int nCnt = 0; nCnt < (int)m_alistGameObj[nCntPri].size(); nCnt++)
		//{
		//	if (m_alistGameObj[nCntPri][nCnt] && !m_alistGameObj[nCntPri][nCnt]->m_bAlive)
		//	{
		//		ReleaseGameObj((PRIORITY)nCntPri, m_alistGameObj[nCntPri][nCnt]);
		//	}
		//}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectManager::DrawAll(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (auto itr = m_alistGameObj[nCntPri].begin(); itr != m_alistGameObj[nCntPri].end(); ++itr)
		{
			(*itr)->Draw();
		}
	}
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̊m��
//--------------------------------------------------------------------------------
void CGameObjectManager::SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	////�z��̊Ԃɋ󂫂�����ꍇ
	//for (int nCnt = 0; nCnt < (int)m_alistGameObj[pri].size(); nCnt++)
	//{
	//	if (m_alistGameObj[pri][nCnt] == NULL)
	//	{
	//		m_alistGameObj[pri][nCnt] = pGameObj;
	//		return;
	//	}
	//}
	////�z��̊Ԃɋ󂫂��Ȃ��ꍇ
	m_alistGameObj[pri].push_back(pGameObj);
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̔j��
//--------------------------------------------------------------------------------
void CGameObjectManager::ReleaseGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	PRIORITY priCopy = pri;
	pGameObj->Uninit();
	delete pGameObj;
	m_alistGameObj[priCopy].remove(pGameObj);

	//for (int nCnt = 0; nCnt < (int)m_alistGameObj[priCopy].size(); nCnt++)
	//{
	//	if (m_alistGameObj[priCopy][nCnt] == pGameObj)
	//	{
	//		m_alistGameObj[priCopy][nCnt]->Uninit();
	//		delete m_alistGameObj[priCopy][nCnt];
	//		m_alistGameObj[priCopy][nCnt] = NULL;
	//		return;
	//	}
	//}
}
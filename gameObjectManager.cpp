//--------------------------------------------------------------------------------
//
//　gameObjManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObjectManager.h"

//gameObject head files
#include "gameObject.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectManager::CGameObjectManager()
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		m_alistGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CGameObjectManager::Init(void)
{
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CGameObjectManager::Uninit(void)
{
	ReleaseAll();
}

//--------------------------------------------------------------------------------
//  リリース処理
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
//  更新処理
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
//  更新処理(描画直前)
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
			{//生きてないオブジェクトを削除
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
//  描画処理
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
//  ゲームオブジェクトの確保
//--------------------------------------------------------------------------------
void CGameObjectManager::SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	////配列の間に空きがある場合
	//for (int nCnt = 0; nCnt < (int)m_alistGameObj[pri].size(); nCnt++)
	//{
	//	if (m_alistGameObj[pri][nCnt] == NULL)
	//	{
	//		m_alistGameObj[pri][nCnt] = pGameObj;
	//		return;
	//	}
	//}
	////配列の間に空きがない場合
	m_alistGameObj[pri].push_back(pGameObj);
}

//--------------------------------------------------------------------------------
//  ゲームオブジェクトの破棄
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
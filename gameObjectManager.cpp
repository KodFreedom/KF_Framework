//--------------------------------------------------------------------------------
//
//　gameObjManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
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
		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CGameObjectManager::Init(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		m_avectorGameObj[nCntPri].clear();
	}
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
//  更新処理
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
//  更新処理(描画直前)
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
//  描画処理
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
//  ゲームオブジェクトの確保
//--------------------------------------------------------------------------------
void CGameObjectManager::SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	//配列の間に空きがある場合
	for (int nCnt = 0; nCnt < (int)m_avectorGameObj[pri].size(); nCnt++)
	{
		if (m_avectorGameObj[pri][nCnt] == NULL)
		{
			m_avectorGameObj[pri][nCnt] = pGameObj;
			return;
		}
	}

	//配列の間に空きがない場合
	m_avectorGameObj[pri].push_back(pGameObj);

}

//--------------------------------------------------------------------------------
//  ゲームオブジェクトの破棄
//--------------------------------------------------------------------------------
void CGameObjectManager::ReleaseGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	PRIORITY priCopy = pri;

	for (int nCnt = 0; nCnt < (int)m_avectorGameObj[pri].size(); nCnt++)
	{
		if (m_avectorGameObj[pri][nCnt] == pGameObj)
		{
			m_avectorGameObj[pri][nCnt]->Uninit();
			delete m_avectorGameObj[pri][nCnt];
			m_avectorGameObj[pri][nCnt] = NULL;
			return;
		}
	}
}
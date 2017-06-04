//--------------------------------------------------------------------------------
//	colliderマネージャ
//　colliderManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "colliderManager.h"
#include "colliderComponent.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CColliderManager::CColliderManager()
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CColliderManager::Init(void)
{

}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CColliderManager::Uninit(void)
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CColliderManager::Update(void)
{
	//Dynamic層のコリジョンごとにまずStatic層と当たり判定して
	//Dynamic層の他のコリジョンと当たり判定する
	//Sphere
	if (!m_alistCollider[DYNAMIC][COL_SPHERE].empty())
	{
		for (COL_ITR itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
		{
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
		}
	}
	
}

////--------------------------------------------------------------------------------
////  DynamicのコリジョンをStaticのコリジョンと当たり判定
////--------------------------------------------------------------------------------
//void CColliderManager::CheckInStatic(const std::list<CColliderComponent*>::iterator& itrThis)
//{
//	
//	if (!m_alistCollision[STATIC].empty())
//	{
//		std::list<CColliderComponent*>::iterator itrTarget;
//
//		for (itrTarget = m_alistCollision[STATIC].begin(); itrTarget != m_alistCollision[STATIC].end(); itrTarget++)
//		{
//			CheckCollision(itrThis, itrTarget);
//		}
//	}
//}
//
////--------------------------------------------------------------------------------
////  DynamicのコリジョンをStaticのコリジョンと当たり判定
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const std::list<CColliderComponent*>::iterator& itrL, const std::list<CColliderComponent*>::iterator& itrR)
//{
//	switch ((*itrL)->GetType())
//	{
//	case CColliderComponent::COL_SPHERE:
//		CSphereColliderComponent* pL = (CSphereColliderComponent*)&(*itrL);
//		break;
//	case CColliderComponent::COL_FIELD:
//		CFieldCollisionComponent* pL = (CFieldCollisionComponent*)&(*itrL);
//	default:
//		break;
//	}
//
//	
//}
//
////--------------------------------------------------------------------------------
////  スフィアをフィールドと当たり判定
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const CSphereColliderComponent& sphere, const CFieldCollisionComponent& field)
//{
//
//}
//
////--------------------------------------------------------------------------------
////  スフィアをフィールドと当たり判定
////--------------------------------------------------------------------------------
//void CColliderManager::CheckCollision(const CFieldCollisionComponent& field, const CSphereColliderComponent& sphere)
//{
//	CheckCollision(sphere, field);
//}

//--------------------------------------------------------------------------------
//  Save処理
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return; }
	COL_ITR itr;
	m_alistCollider[mode][type].push_back(pCollider);
	itr = m_alistCollider[mode][type].begin();
	return itr;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void CColliderManager::ReleaseCollider(const MODE& mode, const COL_TYPE& type, const COL_ITR& itr)
{
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return; }
	m_alistCollider[mode][type].erase(itr);
}
//--------------------------------------------------------------------------------
//	コリジョン判定クラス
//　KF_Collision.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "KF_CollisionSystem.h"
#include "KF_CollisionUtility.h"
#include "KF_PhysicsSystem.h"
#include "gameObject.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "boxColliderComponent.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
#include "rendererDX.h"
#include "textureManager.h"
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CKFCollisionSystem::CKFCollisionSystem()
#ifdef _DEBUG
	: m_pMeshSphere(nullptr)
	, m_pMeshCube(nullptr)
#endif
{
	for (auto& aList : m_alistCollider)
	{
		for (auto& list : aList)
		{
			list.clear();
		}
	}

	m_listField.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Init(void)
{
#ifdef _DEBUG
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	CMain::GetManager()->GetTextureManager()->UseTexture("polygon.png");
	D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_pMeshSphere, nullptr);
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_pMeshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Uninit(void)
{
	for (auto& aList : m_alistCollider)
	{
		for (auto& list : aList)
		{
			list.clear();
		}
	}

	m_listField.clear();

#ifdef _DEBUG
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshCube);
	CMain::GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Update(void)
{
	//Dynamic層のコリジョンごとにまずFStatic層と当たり判定して
	//Dynamic層の他のコリジョンと当たり判定して
	//最後にFieldと当たり判定する
	//Sphere
	for (auto itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); ++itr)
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicSphere(itr, *pSphere); 
#pragma omp section 
			checkWithDynamicAABB(*pSphere); 
#pragma omp section 
			checkWithDynamicOBB(*pSphere);
#pragma omp section 
			checkWithStaticSphere(*pSphere);
#pragma omp section 
			checkWithStaticAABB(*pSphere);
#pragma omp section
			checkWithStaticOBB(*pSphere);
#pragma omp section
			checkWithField(*pSphere);
		}
	}

	//aabb
	for (auto itr = m_alistCollider[DYNAMIC][COL_AABB].begin(); itr != m_alistCollider[DYNAMIC][COL_AABB].end(); ++itr)
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicAABB(itr, *pAABB);
#pragma omp section 
			checkWithDynamicOBB(*pAABB);
#pragma omp section 
			checkWithStaticSphere(*pAABB);
#pragma omp section 
			checkWithStaticAABB(*pAABB);
#pragma omp section 
			checkWithStaticOBB(*pAABB);
#pragma omp section
			checkWithField(*pAABB);
		}
	}

	//obb
	for (auto itr = m_alistCollider[DYNAMIC][COL_OBB].begin(); itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(*itr);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicOBB(itr, *pOBB);
#pragma omp section 
			checkWithStaticSphere(*pOBB);
#pragma omp section 
			checkWithStaticAABB(*pOBB);
#pragma omp section 
			checkWithStaticOBB(*pOBB);
#pragma omp section 
			checkWithField(*pOBB);
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::LateUpdate(void)
{
#ifdef _DEBUG
	auto pDebug = CMain::GetManager()->GetDebugManager();
	pDebug->DisplayAlways("Collider情報 : \n");
	for (int nCntMode = 0; nCntMode < MODE_MAX; ++nCntMode)
	{
		pDebug->DisplayAlways(toString((COL_MODE)nCntMode) + " : ");
		for (int nCntType = 0; nCntType < COL_MAX; ++nCntType)
		{
			int nNumCol = (int)m_alistCollider[nCntMode][nCntType].size();
			pDebug->DisplayAlways(
				toString((COL_TYPE)nCntType) +
				"-" +
				to_string(nNumCol) + "    ");
		}
		pDebug->DisplayAlways('\n');
	}
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].push_back(pCollider);
		return;
	}
	switch (type)
	{
	case CS::COL_FIELD:
		m_listField.push_back(pCollider);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DeregisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].remove(pCollider);
		return;
	}

	switch (type)
	{
	case CS::COL_FIELD:
	{
		m_listField.remove(pCollider);
		break;
	}
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//	関数名：RayCast
//  関数説明：レイキャスト関数、レイと衝突したの最近点を算出する
//	引数：	vOrigin：レイの始点
//			vDirection：レイの向き
//			fDistance：レイの長さ
//			infoOut：衝突点の情報
//			pObjThis：自分のゲームオブジェクト
//	戻り値：衝突フラグ
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::RayCast(const CKFVec3& vOrigin, const CKFVec3& vDirection, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	CKFRay ray = CKFRay(vOrigin, vDirection);
	bool bFind = false;
	CRaycastHitInfo info;
	
#pragma omp parallel sections
	{
#pragma omp section
		{
			if (checkWithDynamicAABB(ray, fDistance, info, pObjThis))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithDynamicOBB(ray, fDistance, info, pObjThis))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithStaticAABB(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithStaticOBB(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}

#pragma omp section
		{
			if (checkWithField(ray, fDistance, info))
			{
				if (bFind && info.m_fDistance < infoOut.m_fDistance)
				{
					infoOut = info;
				}
				bFind = true;
			}
		}
	}

	return bFind;
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DrawCollider(void)
{
	//LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	////sphere
	//LPDIRECT3DTEXTURE9 pTexture = CMain::GetManager()->GetTextureManager()->GetTexture("polygon.png");
	//for (auto itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
	//{
	//	D3DXVECTOR3 vPos = (*itr)->GetWorldPos();
	//	float fRadius = ((CSphereColliderComponent*)(*itr))->GetRadius();
	//	D3DXMATRIX mtx,mtxPos,mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, vPos.x, vPos.y, vPos.z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	pDevice->SetTexture(0, pTexture);
	//	m_pMeshSphere->DrawSubset(0);
	//}

	////Cube
	//for (auto pCol : m_alistCollider[STATIC][COL_AABB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) { continue; }
	//	auto vPos = pCol->GetWorldPos();
	//	auto vHalfSize = ((CAABBColliderComponent*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.m_fX * 2.0f, vHalfSize.m_fY * 2.0f, vHalfSize.m_fZ * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, vPos.m_fX, vPos.m_fY, vPos.m_fZ);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	pDevice->SetTexture(0, pTexture);
	//	m_pMeshCube->DrawSubset(0);
	//}
}
#endif

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  衝突処理
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); ++itr)
	{
		//同じオブジェクトに付いているなら判定しない
		if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);
		CCollisionDetector::CheckSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとAABBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicAABB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(sphere, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//  AABBとAABBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicAABB(const list<CColliderComponent*>::iterator& itrBegin, CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == aabb.GetGameObject()) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckAABBWithAABB(aabb, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//	関数名：checkWithDynamicOBB
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithDynamicAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_AABB])
	{
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == pObjThis) { continue; }
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pAABB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}


//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == sphere.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  AABBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == aabb.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(aabb, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  OBBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(const list<CColliderComponent*>::iterator& itrBegin, COBBColliderComponent& obb)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		//同じオブジェクトに付いているなら判定しない
		if ((*itr)->GetGameObject() == obb.GetGameObject()) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		CCollisionDetector::CheckBoxWithBox(*pOBB, obb);
	}
}

//--------------------------------------------------------------------------------
//	関数名：checkWithDynamicOBB
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithDynamicOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[DYNAMIC][COL_OBB])
	{
		CRaycastHitInfo info;
		//同じオブジェクトに付いているなら判定しない
		if (pCollider->GetGameObject() == pObjThis) { continue; }
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pOBB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとAABの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(*pSphere, aabb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(COBBColliderComponent& obb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_SPHERE])
	{
		auto pSphere = dynamic_cast<CSphereColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(*pSphere, obb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithAABB(sphere, *pAABB);
	}
}

//--------------------------------------------------------------------------------
//  AABBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(CAABBColliderComponent& aabb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckAABBWithAABB(*pAABB, aabb);
	}
}

//--------------------------------------------------------------------------------
//  OBBとAABBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticAABB(COBBColliderComponent& obb)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(*pAABB, obb);
	}
}

//--------------------------------------------------------------------------------
//  OBBとAABBの当たり判定
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithStaticAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[STATIC][COL_AABB])
	{
		auto pAABB = dynamic_cast<CAABBColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pAABB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  boxとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(CBoxColliderComponent& box)
{
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithBox(*pOBB, box);
	}
}

//--------------------------------------------------------------------------------
//  boxとOBBの当たり判定
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithStaticOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;
	for (auto pCollider : m_alistCollider[STATIC][COL_OBB])
	{
		CRaycastHitInfo info;
		auto pOBB = dynamic_cast<COBBColliderComponent*>(pCollider);
		if (CCollisionDetector::CheckRayWithBox(ray, fDistance, *pOBB, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if (info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(CSphereColliderComponent& sphere)
{
	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CCollisionDetector::CheckSphereWithField(sphere, *pField);
	}
}

//--------------------------------------------------------------------------------
//  OBBとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(CBoxColliderComponent& box)
{
	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CCollisionDetector::CheckBoxWithField(box, *pField);
	}
}

//--------------------------------------------------------------------------------
//	関数名：RayCast
//  関数説明：レイキャスト関数、レイと衝突したの最近点を算出する
//	引数：	vOrigin：レイの始点
//			vDirection：レイの向き
//			fDistance：レイの長さ
//			infoOut：衝突点の情報
//			pObjThis：自分のゲームオブジェクト
//	戻り値：衝突フラグ
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkWithField(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut)
{
	bool bFind = false;

	for (auto pCollider : m_listField)
	{
		auto pField = dynamic_cast<CFieldColliderComponent*>(pCollider);
		CRaycastHitInfo info;
		if (CCollisionDetector::CheckRayWithField(ray, fDistance, *pField, info))
		{
			if (!bFind)
			{
				bFind = true;
				infoOut = info;
			}
			else if(info.m_fDistance < infoOut.m_fDistance)
			{
				infoOut = info;
			}
		}
	}

	return bFind;
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//	関数名：toString
//  関数説明：モードのenumをstringに変換
//	引数：	mode
//	戻り値：string
//--------------------------------------------------------------------------------
string CKFCollisionSystem::toString(const COL_MODE& mode)
{
	switch (mode)
	{
	case STATIC:
		return "Static";
	case DYNAMIC:
		return "Dynamic";
	default:
		return "Error";
	}
}

//--------------------------------------------------------------------------------
//	関数名：toString
//  関数説明：タイプのenumをstringに変換
//	引数：	type
//	戻り値：string
//--------------------------------------------------------------------------------
string CKFCollisionSystem::toString(const COL_TYPE& type)
{
	switch (type)
	{
	case COL_SPHERE:
		return "Sphere";
	case COL_AABB:
		return "AABB";
	case COL_OBB:
		return "OBB";
	case COL_PLANE:
		return "Plane";
	case COL_CYLINDER:
		return "Cylinder";
	default:
		return "Error";
	}
}
#endif
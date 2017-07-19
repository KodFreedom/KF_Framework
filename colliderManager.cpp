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
#include "fieldColliderComponent.h"
#include "KF_CollisionSystem.h"
#include "manager.h"
#include "rendererDX.h"
#include "textureManager.h"

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

	m_listField.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CColliderManager::Init(void)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	GetManager()->GetTextureManager()->UseTexture("polygon.png");
	D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_pMeshSphere, NULL);
#endif
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

	m_listField.clear();

#ifdef _DEBUG
	SAFE_RELEASE(m_pMeshSphere);
	GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CColliderManager::Update(void)
{
	//Dynamic層のコリジョンごとにまずFStatic層と当たり判定して
	//Dynamic層の他のコリジョンと当たり判定して
	//最後にFieldと当たり判定する
	//Sphere
	if (!m_alistCollider[DYNAMIC][COL_SPHERE].empty())
	{
		for (COL_ITR itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
		{
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);

			//Dynamic
			CheckWithDynamicSphere(itr, *pSphere);

			//Static
			CheckWithStaticSphere(*pSphere);

			//Field
			CheckWithField(*pSphere);
		}
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CColliderManager::CheckWithDynamicSphere(const COL_ITR& itrBegin, CSphereColliderComponent& sphere)
{
	if (!m_alistCollider[DYNAMIC][COL_SPHERE].empty())
	{
		for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
		{
			//同じオブジェクトに付いているなら判定しない
			if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
			CKFCollisionSystem::CheckCollisionSphereWithSphere(sphere, *pSphere);
		}
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CColliderManager::CheckWithStaticSphere(CSphereColliderComponent& sphere)
{
	if (!m_alistCollider[STATIC][COL_SPHERE].empty())
	{
		for (auto itr = m_alistCollider[STATIC][COL_SPHERE].begin(); itr != m_alistCollider[STATIC][COL_SPHERE].end(); itr++)
		{
			//同じオブジェクトに付いているなら判定しない
			if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
			CSphereColliderComponent* pSphere = (CSphereColliderComponent*)(*itr);
			CKFCollisionSystem::CheckCollisionSphereWithSphere(sphere, *pSphere);
		}
	}
}

//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CColliderManager::CheckWithField(CSphereColliderComponent& sphere)
{	
	if (!m_listField.empty())
	{
		for (COL_ITR itrField = m_listField.begin(); itrField != m_listField.end(); itrField++)
		{
			CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itrField);
			CKFCollisionSystem::CheckCollisionSphereWithField(sphere, *pField);
		}
	}
}

//--------------------------------------------------------------------------------
//  Save処理
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	COL_ITR itr;
	if (mode < 0 || mode >= MODE_MAX || type < 0 || type >= COL_MAX) { return itr; }
	m_alistCollider[mode][type].push_front(pCollider);
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

//--------------------------------------------------------------------------------
//  Save処理(Field)
//--------------------------------------------------------------------------------
CColliderManager::COL_ITR CColliderManager::SaveField(CColliderComponent* pCollider)
{
	COL_ITR itr;
	if (pCollider->GetType() != COL_FIELD) { return itr; }
	m_listField.push_front(pCollider);
	itr = m_listField.begin();
	return itr;
}

//--------------------------------------------------------------------------------
//  破棄処理(Field)
//--------------------------------------------------------------------------------
void CColliderManager::ReleaseField(const COL_ITR& itr)
{
	m_listField.erase(itr);
}

//--------------------------------------------------------------------------------
//  スフィア当たり判定
//--------------------------------------------------------------------------------
bool CColliderManager::SphereCast(const CKFVec3& vCenter, const float& fRadius, HIT_INFO& hitInfoOut)
{
	if (!m_listField.empty())
	{
		for (COL_ITR itrField = m_listField.begin(); itrField != m_listField.end(); itrField++)
		{
			CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itrField);
			CFieldColliderComponent::INFO info = pField->GetPointInfo(vCenter);
			if (!info.bInTheField) { continue; }
			if (info.fHeight > vCenter.m_fY - fRadius)
			{
				hitInfoOut.vNormal = info.vFaceNormal;
				hitInfoOut.vPos = CKFVec3(vCenter.m_fX, info.fHeight, vCenter.m_fZ);
				return true;
			}
		}
	}
	return false;
}

//--------------------------------------------------------------------------------
//  レイ当たり判定
//--------------------------------------------------------------------------------
bool CColliderManager::RayCast(const CKFRay& ray, const float& fLength, HIT_INFO& hitInfoOut)
{
	//CKFVec3 vBegin = ray.m_vOrigin;
	//CKFVec3 vEnd = ray.m_vOrigin + ray.m_vDirection * fLength;

	//if (!m_listField.empty())
	//{
	//	for (COL_ITR itrField = m_listField.begin(); itrField != m_listField.end(); itrField++)
	//	{
	//		CFieldColliderComponent* pField = (CFieldColliderComponent*)(*itrField);
	//	}
	//}
	return false;
}

#ifdef _DEBUG
void CColliderManager::DrawCollider(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	////sphere
	//LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture("polygon.png");
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
}
#endif
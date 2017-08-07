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
#include "KF_PhysicsSystem.h"
#include "gameObject.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "boxColliderComponent.h"
#include "OBBColliderComponent.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
#include "rendererDX.h"
#include "textureManager.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
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
void CKFCollisionSystem::Init(void)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	GetManager()->GetTextureManager()->UseTexture("polygon.png");
	D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_pMeshSphere, nullptr);
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_pMeshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::Uninit(void)
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
	SAFE_RELEASE(m_pMeshCube);
	GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
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
		CSphereColliderComponent* pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);

		//Dynamic
		checkWithDynamicSphere(itr, *pSphere);
		checkWithDynamicOBB(*pSphere);

		//Static
		checkWithStaticSphere(*pSphere);
		checkWithStaticOBB(*pSphere);

		//Field
		checkWithField(*pSphere);
	}

	//obb
	for (auto itr = m_alistCollider[DYNAMIC][COL_OBB].begin(); itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		COBBColliderComponent* pOBB = dynamic_cast<COBBColliderComponent*>(*itr);

		//Dynamic
		checkWithDynamicOBB(itr, *pOBB);

		//Static
		checkWithStaticSphere(*pOBB);
		checkWithStaticOBB(*pOBB);

		//Field
		checkWithField(*pOBB);
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::LateUpdate(void)
{
#ifndef _DEBUG
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}

	m_listField.clear();
#endif // !_DEBUG
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFCollisionSystem::RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider)
{
	if (type < CS::COL_MAX)
	{
		m_alistCollider[mode][type].push_back(pCollider);
	}
	else
	{
		switch (type)
		{
		case CS::COL_FIELD:
			m_listField.push_back(pCollider);
			break;
		default:
			break;
		}
	}
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CKFCollisionSystem::DrawCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	//sphere
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture("polygon.png");
	for (auto itr = m_alistCollider[DYNAMIC][COL_SPHERE].begin(); itr != m_alistCollider[DYNAMIC][COL_SPHERE].end(); itr++)
	{
		D3DXVECTOR3 vPos = (*itr)->GetWorldPos();
		float fRadius = ((CSphereColliderComponent*)(*itr))->GetRadius();
		D3DXMATRIX mtx,mtxPos,mtxScale;
		D3DXMatrixIdentity(&mtx);
		D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
		mtx *= mtxScale;
		D3DXMatrixTranslation(&mtxPos, vPos.x, vPos.y, vPos.z);
		mtx *= mtxPos;
		pDevice->SetTransform(D3DTS_WORLD, &mtx);
		pDevice->SetTexture(0, pTexture);
		m_pMeshSphere->DrawSubset(0);
	}

	////Cube
	//for (auto itr = m_alistCollider[DYNAMIC][COL_OBB].begin(); itr != m_alistCollider[DYNAMIC][COL_OBB].end(); itr++)
	//{
	//	CKFVec3 vHalfSize = ((COBBColliderComponent*)(*itr))->GetHalfSize();
	//	D3DXMATRIX mtx, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	pDevice->SetTexture(0, pTexture);
	//	m_pMeshSphere->DrawSubset(0);
	//}

	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		for (int nCntType = 0; nCntType < COL_MAX; nCntType++)
		{
			m_alistCollider[nCntMode][nCntType].clear();
		}
	}

	m_listField.clear();
}
#endif

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
		CSphereColliderComponent* pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);
		checkCollisionSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(CSphereColliderComponent& sphere)
{
	for (auto itr = m_alistCollider[DYNAMIC][COL_OBB].begin(); itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		//同じオブジェクトに付いているなら判定しない
		if ((*itr)->GetGameObject() == sphere.GetGameObject()) { continue; }
		COBBColliderComponent* pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		checkCollisionSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithDynamicOBB(const list<CColliderComponent*>::iterator& itrBegin, COBBColliderComponent& obb)
{
	for (auto itr = itrBegin; itr != m_alistCollider[DYNAMIC][COL_OBB].end(); ++itr)
	{
		//同じオブジェクトに付いているなら判定しない
		if ((*itr)->GetGameObject() == obb.GetGameObject()) { continue; }
		COBBColliderComponent* pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		checkCollisionBoxWithBox(*pOBB, obb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(CSphereColliderComponent& sphere)
{
	for (auto itr = m_alistCollider[STATIC][COL_SPHERE].begin(); itr != m_alistCollider[STATIC][COL_SPHERE].end(); ++itr)
	{
		CSphereColliderComponent* pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);
		checkCollisionSphereWithSphere(sphere, *pSphere);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticSphere(COBBColliderComponent& obb)
{
	for (auto itr = m_alistCollider[STATIC][COL_SPHERE].begin(); itr != m_alistCollider[STATIC][COL_SPHERE].end(); ++itr)
	{
		CSphereColliderComponent* pSphere = dynamic_cast<CSphereColliderComponent*>(*itr);
		checkCollisionSphereWithOBB(*pSphere, obb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(CSphereColliderComponent& sphere)
{
	for (auto itr = m_alistCollider[STATIC][COL_OBB].begin(); itr != m_alistCollider[STATIC][COL_OBB].end(); ++itr)
	{
		COBBColliderComponent* pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		checkCollisionSphereWithOBB(sphere, *pOBB);
	}
}

//--------------------------------------------------------------------------------
//  OBBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithStaticOBB(COBBColliderComponent& obb)
{
	for (auto itr = m_alistCollider[STATIC][COL_OBB].begin(); itr != m_alistCollider[STATIC][COL_OBB].end(); ++itr)
	{
		COBBColliderComponent* pOBB = dynamic_cast<COBBColliderComponent*>(*itr);
		checkCollisionBoxWithBox(*pOBB, obb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(CSphereColliderComponent& sphere)
{
	for (auto itr = m_listField.begin(); itr != m_listField.end(); ++itr)
	{
		CFieldColliderComponent* pField = dynamic_cast<CFieldColliderComponent*>(*itr);
		checkCollisionSphereWithField(sphere, *pField);
	}
}

//--------------------------------------------------------------------------------
//  OBBとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkWithField(COBBColliderComponent& obb)
{
	for (auto itr = m_listField.begin(); itr != m_listField.end(); ++itr)
	{
		CFieldColliderComponent* pField = dynamic_cast<CFieldColliderComponent*>(*itr);
		checkCollisionOBBWithField(obb, *pField);
	}
}

//--------------------------------------------------------------------------------
//
//  衝突判定関数
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR)
{
	CKFVec3 vSLPos = sphereL.GetWorldPos();
	float fSLRadius = sphereL.GetRadius();
	CKFVec3 vSRPos = sphereR.GetWorldPos();
	float fSRRadius = sphereR.GetRadius();
	CKFVec3 vMidLine = vSLPos - vSRPos;
	float fDisMin = fSLRadius + fSRRadius;
	float fDisSqr = CKFMath::VecMagnitudeSquare(vMidLine);

	if (fDisSqr >= fDisMin * fDisMin) { return; }

	if (sphereL.IsTrigger() || sphereR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto list = sphereL.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphereL, sphereR);
		}

		list = sphereR.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphereR, sphereL);
		}
		return;
	}

	CCollision collision;
		
	//衝突点の算出
	collision.m_vCollisionPos = vSRPos + vMidLine * 0.5f;

	//衝突深度の算出
	float fDis = sqrtf(fDisSqr);
	collision.m_fPenetration = fDisMin - fDis;

	//衝突法線の算出
	collision.m_vCollisionNormal = vMidLine / fDis;

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = sphereL.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision.m_vCollisionNormal *= -1.0f;
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo info;
	info.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	info.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	info.m_listCollision.push_back(&collision);
	auto list = sphereL.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphereL, info);
	}

	list = sphereR.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphereR, info);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkCollisionSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb)
{
	CKFVec3 vSPos = sphere.GetWorldPos();
	float fSRadius = sphere.GetRadius();
	CKFVec3 vBHalfSize = obb.GetHalfSize();
	CKFMtx44 mtxBox = obb.GetMatrixWorld();
	CKFVec3 vRealSPos = CKFMath::TransformInverse(mtxBox, vSPos);
	
	//分離軸チェック
	if (fabsf(vRealSPos.m_fX) - fSRadius > vBHalfSize.m_fX
		|| fabsf(vRealSPos.m_fY) - fSRadius > vBHalfSize.m_fY
		|| fabsf(vRealSPos.m_fZ) - fSRadius > vBHalfSize.m_fZ)
	{
		return;
	}

	CKFVec3 vClosestPos;
	float fDis;

	//OBBとスフィアの最近点の算出
	fDis = vRealSPos.m_fX;
	if (fDis > vBHalfSize.m_fX) { fDis = vBHalfSize.m_fX; }
	else if (fDis < -vBHalfSize.m_fX) { fDis = -vBHalfSize.m_fX; }
	vClosestPos.m_fX = fDis;

	fDis = vRealSPos.m_fY;
	if (fDis > vBHalfSize.m_fY) { fDis = vBHalfSize.m_fY; }
	else if (fDis < -vBHalfSize.m_fY) { fDis = -vBHalfSize.m_fY; }
	vClosestPos.m_fY = fDis;

	fDis = vRealSPos.m_fZ;
	if (fDis > vBHalfSize.m_fZ) { fDis = vBHalfSize.m_fZ; }
	else if (fDis < -vBHalfSize.m_fZ) { fDis = -vBHalfSize.m_fZ; }
	vClosestPos.m_fZ = fDis;

	//衝突検知
	float fDisSqr = CKFMath::VecMagnitudeSquare(vClosestPos - vRealSPos);
	if (fDisSqr >= fSRadius * fSRadius) { return; }

	//衝突情報
	vClosestPos = CKFMath::Vec3TransformCoord(vClosestPos, mtxBox);
	CCollision collision;
	collision.m_vCollisionNormal = vSPos - vClosestPos;
	CKFMath::VecNormalize(collision.m_vCollisionNormal);
	collision.m_vCollisionPos = vClosestPos;
	collision.m_fPenetration = fSRadius - sqrtf(fDisSqr);
	
	//リジッドボディの取得
	CRigidbodyComponent* pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = obb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision.m_vCollisionNormal *= -1.0f;
		collision.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo info;
	info.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	info.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	info.m_listCollision.push_back(&collision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphere, info);
	}

	list = obb.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(obb, info);
	}
}

//--------------------------------------------------------------------------------
//  checkCollisionBoxWithBox
//	BoxとBoxの当たり判定（いずれかが回転ある（AABBではない））
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkCollisionBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR)
{

}

//--------------------------------------------------------------------------------
//
//  フィールド
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	CKFVec3 vSpherePos = sphere.GetWorldPos();
	float fSphereRadius = sphere.GetRadius();
	CFieldColliderComponent::INFO info = field.GetPointInfo(vSpherePos);

	if (info.bInTheField == false) { return; }

	//スフィア中心とポリゴンの距離の算出
	float fDis = vSpherePos.m_fY - fSphereRadius - info.fHeight;

	if (fDis >= 0.0f) { return; }

	if (sphere.IsTrigger())
	{
		auto list = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphere, field);
		}
		return;
	}
	CCollision collision;

	//衝突点の算出
	collision.m_vCollisionPos = CKFVec3(vSpherePos.m_fX, info.fHeight, vSpherePos.m_fZ);

	//衝突深度の算出
	collision.m_fPenetration = -fDis;

	//衝突法線の算出
	collision.m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f);

	//リジッドボディの取得
	C3DRigidbodyComponent* p3DRB = dynamic_cast<C3DRigidbodyComponent*>(sphere.GetGameObject()->GetRigidbodyComponent());
	p3DRB->SetOnGround(true);
	collision.m_pRigidBodyOne = p3DRB;
	collision.m_pRigidBodyTwo = nullptr;

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	cInfo.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	cInfo.m_listCollision.push_back(&collision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(sphere, cInfo);
	}

	list = field.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(field, cInfo);
	}
	/*
	//CGameObject* pSphereObj = sphere.GetGameObject();

	////位置調節
	//CKFVec3 vMovement = CKFVec3(0.0f, info.fHeight + fSphereRadius - vSpherePos.m_fY, 0.0f);
	//pSphereObj->GetTransformComponent()->MovePosNext(vMovement);

	////回転調節
	////CKFVec3 vUpNext = CKFMath::LerpNormal(pSphereObj->GetUpNext(), info.vFaceNormal, 0.2f);
	////pSphereObj->RotByUp(vUpNext);

	////速度調節
	//CRigidbodyComponent* pRB = sphere.GetGameObject()->GetRigidbodyComponent();
	//if (pRB->GetType() == CRigidbodyComponent::RB_3D)
	//{
	//	C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;
	//	p3DRB->SetOnGround(true);
	//	CKFVec3 vVelocity = p3DRB->GetVelocity();
	//	vVelocity.m_fY = 0.0f;
	//	p3DRB->SetVelocity(vVelocity);
	//}
	*/
}

//--------------------------------------------------------------------------------
//  OBBとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::checkCollisionOBBWithField(COBBColliderComponent& obb, CFieldColliderComponent& field)
{
	auto listOBBVtx = obb.GetWorldVertexes();
	
	//回転なしの場合一番深いの頂点を案出する
	bool bFind = false;
	CCollision collision;
	for (auto itr = listOBBVtx.begin(); itr != listOBBVtx.end(); ++itr)
	{
		auto info = field.GetPointInfo((*itr));
		if (info.bInTheField == false) { continue; }

		float fDis = (*itr).m_fY - info.fHeight;
		if (fDis >= 0.0f) { continue; }

		if (!bFind)
		{
			bFind = true;

			//衝突点の算出
			collision.m_vCollisionPos = CKFVec3((*itr).m_fX, info.fHeight, (*itr).m_fZ);

			//衝突深度の算出
			collision.m_fPenetration = -fDis;

			//衝突法線の算出
			collision.m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f);

			//リジッドボディの取得
			C3DRigidbodyComponent* p3DRB = dynamic_cast<C3DRigidbodyComponent*>(obb.GetGameObject()->GetRigidbodyComponent());
			p3DRB->SetOnGround(true);
			collision.m_pRigidBodyOne = p3DRB;
			collision.m_pRigidBodyTwo = nullptr;
		}
		else
		{//深度を比べる
			if (-fDis <= collision.m_fPenetration) { continue; }

			//衝突点の算出
			collision.m_vCollisionPos = CKFVec3((*itr).m_fX, info.fHeight, (*itr).m_fZ);

			//衝突深度の算出
			collision.m_fPenetration = -fDis;
		}
	}

	if (!bFind) { return; }

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegistryCollision(collision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pRigidBodyOne = collision.m_pRigidBodyOne;
	cInfo.m_pRigidBodyTwo = collision.m_pRigidBodyTwo;
	cInfo.m_listCollision.push_back(&collision);
	auto list = obb.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(obb, cInfo);
	}

	list = field.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(field, cInfo);
	}
}

//--------------------------------------------------------------------------------
//  Box(AABBとOBB)のハーフサイズをとある軸に投影して長さを算出する
//--------------------------------------------------------------------------------
float CKFCollisionSystem::transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis)
{
	const CKFVec3& vHalfSize = box.GetHalfSize();
	const CKFMtx44& mtxBox = box.GetMatrixWorld();
	float fAnswer = vHalfSize.m_fX * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[0][0], mtxBox.m_af[0][1], mtxBox.m_af[0][2])))
		+ vHalfSize.m_fY * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[1][0], mtxBox.m_af[1][1], mtxBox.m_af[1][2])))
		+ vHalfSize.m_fZ * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[2][0], mtxBox.m_af[2][1], mtxBox.m_af[2][2])));
	return fAnswer;
}

//--------------------------------------------------------------------------------
//  checkOverlapOnAxis
//	ボックス同士がとある軸に重ねてるかどうかをチェックする
//--------------------------------------------------------------------------------
bool CKFCollisionSystem::checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis)
{
	//軸上の長さを算出する
	float fHalfDisL = transformBoxToAxis(boxL, vAxis);
	float fHalfDisR = transformBoxToAxis(boxR, vAxis);

	//軸上の中心間の距離を算出する
	CKFVec3 vLtoR = boxL.GetWorldPos() - boxR.GetWorldPos();
	float fDis = fabsf(CKFMath::Vec3Dot(vLtoR, vAxis));

	//重ねてるかどうかをチェックする
	bool bAns = fDis < (fHalfDisL + fHalfDisR);
	return bAns;
}
//--------------------------------------------------------------------------------
//	コリジョン計算関数
//　KF_CollisionUtility.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "KF_CollisionUtility.h"
#include "KF_PhysicsSystem.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "boxColliderComponent.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"
#include "gameObject.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//
//  衝突判定関数
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR)
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

	CCollision* pCollision = new CCollision;

	//衝突点の算出
	pCollision->m_vCollisionPos = vSRPos + vMidLine * 0.5f;

	//衝突深度の算出
	float fDis = sqrtf(fDisSqr);
	pCollision->m_fPenetration = fDisMin - fDis;

	//衝突法線の算出
	pCollision->m_vCollisionNormal = vMidLine / fDis;

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = sphereL.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphereL;
	info.m_pCollider = &sphereR;
	info.m_listCollision.push_back(pCollision);
	auto list = sphereL.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}

	info.m_pColliderThis = &sphereR;
	info.m_pCollider = &sphereL;
	list = sphereR.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb)
{
	const CKFVec3& vSPos = sphere.GetWorldPos();
	const float& fSRadius = sphere.GetRadius();
	const CKFVec3& vBHalfSize = aabb.GetHalfSize();
	const CKFVec3& vBPos = aabb.GetWorldPos();
	CKFVec3 vRealSPos = vSPos - vBPos;

	//分離軸チェック
	if (fabsf(vRealSPos.m_fX) - fSRadius > vBHalfSize.m_fX
		|| fabsf(vRealSPos.m_fY) - fSRadius > vBHalfSize.m_fY
		|| fabsf(vRealSPos.m_fZ) - fSRadius > vBHalfSize.m_fZ)
	{
		return;
	}

	CKFVec3 vClosestPos;
	float fDis;

	//AABBとスフィアの最近点の算出
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

	//OnTrigger
	if (sphere.IsTrigger() || aabb.IsTrigger())
	{//トリガーだったら物理処理しない
		auto list = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphere, aabb);
		}

		list = aabb.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(aabb, sphere);
		}
		return;
	}

	//衝突情報
	vClosestPos = vClosestPos + vBPos;
	CCollision* pCollision = new CCollision;
	pCollision->m_vCollisionNormal = vSPos - vClosestPos;
	CKFMath::VecNormalize(pCollision->m_vCollisionNormal);
	pCollision->m_vCollisionPos = vClosestPos;
	pCollision->m_fPenetration = fSRadius - sqrtf(fDisSqr);

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = aabb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphere;
	info.m_pCollider = &aabb;
	info.m_listCollision.push_back(pCollision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}

	info.m_pColliderThis = &aabb;
	info.m_pCollider = &sphere;
	list = aabb.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb)
{
	const CKFVec3& vSPos = sphere.GetWorldPos();
	const float& fSRadius = sphere.GetRadius();
	const CKFVec3& vBHalfSize = obb.GetHalfSize();
	const CKFMtx44& mtxBox = obb.GetMatrixWorld();
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

	//OnTrigger
	if (sphere.IsTrigger() || obb.IsTrigger())
	{//トリガーだったら物理処理しない
		auto list = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(sphere, obb);
		}

		list = obb.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(obb, sphere);
		}
		return;
	}

	//衝突情報
	vClosestPos = CKFMath::Vec3TransformCoord(vClosestPos, mtxBox);
	CCollision* pCollision = new CCollision;
	pCollision->m_vCollisionNormal = vSPos - vClosestPos;
	CKFMath::VecNormalize(pCollision->m_vCollisionNormal);
	pCollision->m_vCollisionPos = vClosestPos;
	pCollision->m_fPenetration = fSRadius - sqrtf(fDisSqr);

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = obb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphere;
	info.m_pCollider = &obb;
	info.m_listCollision.push_back(pCollision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}

	info.m_pColliderThis = &obb;
	info.m_pCollider = &sphere;
	list = obb.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}
}

//--------------------------------------------------------------------------------
//  AABBとAABBの当たり判定
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR)
{
	if (!checkOverlapAABB(aabbL, aabbR)) { return; }

	//OnTrigger
	if (aabbL.IsTrigger() || aabbR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto list = aabbL.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(aabbL, aabbR);
		}

		list = aabbR.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(aabbR, aabbL);
		}
		return;
	}

	//aabbLのすべての頂点とaabbRと判定し、めり込みが一番深いの頂点を洗い出す
	bool bFindL = false;
	CCollision collisionDepthMaxL;
	auto& listVtxBoxL = aabbL.GetWorldVertexes();
	for (auto itr = listVtxBoxL.begin(); itr != listVtxBoxL.end(); ++itr)
	{
		CCollision collision;
		if (checkPointWithAABB(collision, *itr, aabbR))
		{
			if (!bFindL)
			{
				bFindL = true;
				collisionDepthMaxL = collision;
			}
			else if (collision.m_fPenetration > collisionDepthMaxL.m_fPenetration)
			{
				collisionDepthMaxL = collision;
			}
		}
	}

	//aabbRのすべての頂点とaabbLと判定し、めり込みが一番深いの頂点を洗い出す
	bool bFindR = false;
	CCollision collisionDepthMaxR;

	if (!bFindL)
	{
		auto& listVtxBoxR = aabbR.GetWorldVertexes();
		for (auto itr = listVtxBoxR.begin(); itr != listVtxBoxR.end(); ++itr)
		{
			CCollision collision;
			if (checkPointWithAABB(collision, *itr, aabbL))
			{
				if (!bFindR)
				{
					bFindR = true;
					collisionDepthMaxR = collision;
				}
				else if (collision.m_fPenetration > collisionDepthMaxR.m_fPenetration)
				{
					collisionDepthMaxR = collision;
				}
			}
		}
	}

	if (!bFindL && !bFindR) { return; } // 万が一のため

	CCollisionInfo info;

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = aabbL.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = aabbR.GetGameObject()->GetRigidbodyComponent();

	if (bFindL)
	{
		if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

			if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxL.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxL.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}

		//物理演算システムにレジストリ
		CCollision* pCollision = new CCollision;
		*pCollision = collisionDepthMaxL;
		GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxL);
	}

	if (bFindR)
	{
		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);

			if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxR.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxR.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
		}

		//物理演算システムにレジストリ
		CCollision* pCollision = new CCollision;
		*pCollision = collisionDepthMaxR;
		GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxR);
	}

	//OnCollision
	info.m_pColliderThis = &aabbL;
	info.m_pCollider = &aabbR;
	info.m_listCollision.push_back(&collisionDepthMaxL);
	auto list = aabbL.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}

	info.m_pColliderThis = &aabbR;
	info.m_pCollider = &aabbL;
	list = aabbR.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}
}

//--------------------------------------------------------------------------------
//  checkCollisionBoxWithBox
//	BoxとBoxの当たり判定（いずれかが回転ある（AABBではない））
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR)
{
	//ボックスがxyz軸上に重ねてるかどうかをチェックする
	if (!checkOverlapOnAxis(boxL, boxR, CKFVec3(1.0f, 0.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, CKFVec3(0.0f, 1.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, CKFVec3(0.0f, 0.0f, 1.0f)))
	{
		return;
	}

	bool bFindL = false;
	CCollision collisionDepthMaxL;

	//boxLのすべての頂点とboxRと判定し、めり込みが一番深いの頂点を洗い出す
	auto& listVtxBoxL = boxL.GetWorldVertexes();
	for (auto itr = listVtxBoxL.begin(); itr != listVtxBoxL.end(); ++itr)
	{
		CCollision collision;
		if (checkPointWithBox(collision, *itr, boxR))
		{
			if (!bFindL)
			{
				bFindL = true;
				collisionDepthMaxL = collision;
			}
			else if (collision.m_fPenetration > collisionDepthMaxL.m_fPenetration)
			{
				collisionDepthMaxL = collision;
			}
		}
	}

	bool bFindR = false;
	CCollision collisionDepthMaxR;

	//boxRのすべての頂点とboxLと判定し、めり込みが一番深いの頂点を洗い出す
	auto& listVtxBoxR = boxR.GetWorldVertexes();
	for (auto itr = listVtxBoxR.begin(); itr != listVtxBoxR.end(); ++itr)
	{
		CCollision collision;
		if (checkPointWithBox(collision, *itr, boxL))
		{
			if (!bFindR)
			{
				bFindR = true;
				collisionDepthMaxR = collision;
			}
			else if (collision.m_fPenetration > collisionDepthMaxR.m_fPenetration)
			{
				collisionDepthMaxR = collision;
			}
		}
	}

	if (!bFindL && !bFindR) { return; }

	//OnTrigger
	if (boxL.IsTrigger() || boxR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto list = boxL.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(boxL, boxR);
		}

		list = boxR.GetGameObject()->GetBehaviorComponent();
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			(*itr)->OnTrigger(boxR, boxL);
		}
		return;
	}

	CCollisionInfo info;

	//リジッドボディの取得
	CRigidbodyComponent* pRBL = boxL.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = boxR.GetGameObject()->GetRigidbodyComponent();

	if (bFindL)
	{
		if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

			if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxL.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxL.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}

		//物理演算システムにレジストリ
		CCollision* pCollision = new CCollision;
		*pCollision = collisionDepthMaxL;
		GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxL);
	}

	if (bFindR)
	{
		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);

			if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxR.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxR.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
		}

		//物理演算システムにレジストリ
		CCollision* pCollision = new CCollision;
		*pCollision = collisionDepthMaxR;
		GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxR);
	}

	//OnCollision
	info.m_pColliderThis = &boxL;
	info.m_pCollider = &boxR;
	info.m_listCollision.push_back(&collisionDepthMaxL);
	auto list = boxL.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}

	info.m_pColliderThis = &boxR;
	info.m_pCollider = &boxL;
	list = boxR.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(info);
	}
}

//--------------------------------------------------------------------------------
//
//  フィールド
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
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
	CCollision* pCollision = new CCollision;

	//衝突点の算出
	pCollision->m_vCollisionPos = CKFVec3(vSpherePos.m_fX, info.fHeight, vSpherePos.m_fZ);

	//衝突深度の算出
	pCollision->m_fPenetration = -fDis;

	//衝突法線の算出
	pCollision->m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f);

	//リジッドボディの取得
	C3DRigidbodyComponent* p3DRB = dynamic_cast<C3DRigidbodyComponent*>(sphere.GetGameObject()->GetRigidbodyComponent());
	pCollision->m_pRigidBodyOne = p3DRB;
	pCollision->m_pRigidBodyTwo = nullptr;

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pColliderThis = &sphere;
	cInfo.m_pCollider = &field;
	cInfo.m_listCollision.push_back(pCollision);
	auto list = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(cInfo);
	}

	cInfo.m_pColliderThis = &field;
	cInfo.m_pCollider = &sphere;
	list = field.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(cInfo);
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
void CCollisionDetector::CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field)
{
	auto listOBBVtx = box.GetWorldVertexes();

	//回転なしの場合一番深いの頂点を案出する
	bool bFind = false;
	CCollision* pCollision = new CCollision;
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
			pCollision->m_vCollisionPos = CKFVec3((*itr).m_fX, info.fHeight, (*itr).m_fZ);

			//衝突深度の算出
			pCollision->m_fPenetration = -fDis;

			//衝突法線の算出
			pCollision->m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f);

			//リジッドボディの取得
			C3DRigidbodyComponent* p3DRB = dynamic_cast<C3DRigidbodyComponent*>(box.GetGameObject()->GetRigidbodyComponent());
			pCollision->m_pRigidBodyOne = p3DRB;
			pCollision->m_pRigidBodyTwo = nullptr;
		}
		else
		{//深度を比べる
			if (-fDis <= pCollision->m_fPenetration) { continue; }

			//衝突点の算出
			pCollision->m_vCollisionPos = CKFVec3((*itr).m_fX, info.fHeight, (*itr).m_fZ);

			//衝突深度の算出
			pCollision->m_fPenetration = -fDis;
		}
	}

	if (!bFind) { return; }

	//物理演算システムにレジストリ
	GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pColliderThis = &box;
	cInfo.m_pCollider = &field;
	cInfo.m_listCollision.push_back(pCollision);
	auto list = box.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(cInfo);
	}

	cInfo.m_pColliderThis = &field;
	cInfo.m_pCollider = &box;
	list = field.GetGameObject()->GetBehaviorComponent();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->OnCollision(cInfo);
	}
}

//--------------------------------------------------------------------------------
//
//  レイ
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：CheckRayWithField
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithBox(const CKFRay& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut)
{
	CCollision collision;
	if (checkPointWithBox(collision, ray.m_vOrigin, box))
	{
		infoOut.m_vNormal = collision.m_vCollisionNormal;
		infoOut.m_vPos = ray.m_vOrigin;
		infoOut.m_pCollider = &box;
		infoOut.m_fDistance = collision.m_fPenetration;
		return true;
	}
	
	auto vRayEnd = ray.m_vOrigin + ray.m_vDirection * fDistance;
	if (checkPointWithBox(collision, vRayEnd, box))
	{
		infoOut.m_vNormal = collision.m_vCollisionNormal;
		infoOut.m_vPos = vRayEnd;
		infoOut.m_pCollider = &box;
		infoOut.m_fDistance = collision.m_fPenetration;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//	関数名：CheckRayWithField
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithSphere(const CKFRay& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut)
{
	CKFVec3 vOriginToSphere;
	auto vSpherePos = sphere.GetWorldPos();
	auto fRadius = sphere.GetRadius();
	float fWorkA, fWorkB, fTimeA, fTimeB;
	float fDiscriminant;

	vOriginToSphere = ray.m_vOrigin - vSpherePos;
	fWorkA = 2.0f * CKFMath::Vec3Dot(ray.m_vDirection, vOriginToSphere);
	fWorkB = CKFMath::Vec3Dot(vOriginToSphere, vOriginToSphere) - fRadius * fRadius;

	fDiscriminant = fWorkA * fWorkA - 4.0f * fWorkB;

	if (fDiscriminant < 0.0f)
	{
		return false;
	}

	fDiscriminant = sqrtf(fDiscriminant);

	fTimeA = (-fWorkA + fDiscriminant) / 2.0f;
	fTimeB = (-fWorkA - fDiscriminant) / 2.0f;

	if (fTimeA < 0.0f && fTimeB < 0.0f) { return false; }
	
	//最短時間を保存
	fTimeA = fTimeA < 0.0f ? fTimeB : fTimeA;
	fTimeB = fTimeB < 0.0f ? fTimeA : fTimeB;
	auto fTimingMin = fTimeA <= fTimeB ? fTimeA : fTimeB;

	if (fTimingMin > fDistance) { return false; }

	infoOut.m_fDistance = fTimingMin;
	infoOut.m_pCollider = &sphere;
	infoOut.m_vPos = ray.m_vOrigin + ray.m_vDirection * fTimingMin;
	infoOut.m_vNormal = infoOut.m_vPos - vSpherePos;
	CKFMath::VecNormalize(infoOut.m_vNormal);
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：CheckRayWithField
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithField(const CKFRay& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut)
{
	//auto vRayMax = ray.m_vOrigin + ray.m_vDirection * fDistance;
	//int nNumVtxX = 0;
	//int nNumVtxZ = 0;
	//vector<CKFVec3> vecVtx;
	//if (!field.GetVtxByRange(ray.m_vOrigin, vRayMax, nNumVtxX, nNumVtxZ, vecVtx))
	//{
	//	return false;
	//}

	//一時採用
	auto info = field.GetPointInfo(ray.m_vOrigin);
	if (!info.bInTheField) { return false; }

	auto fDis = ray.m_vOrigin.m_fY - info.fHeight;
	if (fDis >= fDistance || fDis < 0.0f) { return false; }
	infoOut.m_fDistance = fDis;
	infoOut.m_vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
	infoOut.m_pCollider = &field;
	return true;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  checkCollisionBoxWithBox
//	BoxとBoxの当たり判定（いずれかが回転ある（AABBではない））
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb)
{
	const CKFVec3& vBPos = aabb.GetWorldPos();
	const CKFVec3& vBHalfSize = aabb.GetHalfSize();
	CKFVec3 vRealPoint = vPoint - vBPos;

	float fDepthMin = vBHalfSize.m_fX - fabsf(vRealPoint.m_fX);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.m_vCollisionNormal = CKFVec3(1.0f, 0.0f, 0.0f)
		* (vRealPoint.m_fX < 0.0f ? -1.0f : 1.0f);

	float fDepth = vBHalfSize.m_fY - fabsf(vRealPoint.m_fY);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(0.0f, 1.0f, 0.0f)
			* (vRealPoint.m_fY < 0.0f ? -1.0f : 1.0f);
	}

	fDepth = vBHalfSize.m_fZ - fabsf(vRealPoint.m_fZ);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(0.0f, 0.0f, 1.0f)
			* (vRealPoint.m_fZ < 0.0f ? -1.0f : 1.0f);
	}

	collisionOut.m_fPenetration = fDepthMin;
	collisionOut.m_vCollisionPos = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//  checkCollisionBoxWithBox
//	BoxとBoxの当たり判定（いずれかが回転ある（AABBではない））
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box)
{
	const CKFMtx44& mtxBox = box.GetMatrixWorld();
	const CKFVec3& vBHalfSize = box.GetHalfSize();
	CKFVec3 vRealPoint = CKFMath::TransformInverse(mtxBox, vPoint);

	float fDepthMin = vBHalfSize.m_fX - fabsf(vRealPoint.m_fX);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[0][0], mtxBox.m_af[0][1], mtxBox.m_af[0][2])
		* (vRealPoint.m_fX < 0.0f ? -1.0f : 1.0f);

	float fDepth = vBHalfSize.m_fY - fabsf(vRealPoint.m_fY);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[1][0], mtxBox.m_af[1][1], mtxBox.m_af[1][2])
			* (vRealPoint.m_fY < 0.0f ? -1.0f : 1.0f);
	}

	fDepth = vBHalfSize.m_fZ - fabsf(vRealPoint.m_fZ);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[2][0], mtxBox.m_af[2][1], mtxBox.m_af[2][2])
			* (vRealPoint.m_fZ < 0.0f ? -1.0f : 1.0f);
	}

	collisionOut.m_fPenetration = fDepthMin;
	collisionOut.m_vCollisionPos = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：checkLineWithLine
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkLineWithLine(const CKFVec2& vLA, const CKFVec2& vLB, const CKFVec2& vRA, const CKFVec2& vRB, CKFVec2& vOut)
{
	auto vLineL = vLB - vLA;
	auto vLineR = vRB - vRA;

	auto fSlopeL = (vLA.m_fX - vLB.m_fX) == 0.0f ? 0.0f
		: (vLA.m_fY - vLB.m_fY) / (vLA.m_fX - vLB.m_fX);
	auto fAddL = vLA.m_fY - fSlopeL * vLA.m_fX;
	auto fSlopeR = (vRA.m_fX - vRB.m_fX) == 0.0f ? 0.0f
		: (vRA.m_fY - vRB.m_fY) / (vRA.m_fX - vRB.m_fX);
	auto fAddR = vRA.m_fY - fSlopeL * vRA.m_fX;
	
	//平行
	if (fSlopeL == fSlopeR) { return false; }
	
	//交点の算出
	vOut.m_fX = (fAddR - fAddL) / (fSlopeL - fSlopeR);
	vOut.m_fY = fSlopeL * vOut.m_fX + fAddL;

	//交点がラインの範囲内にあるかをチェック
	//LineL
	auto vDirL = vOut - vLA;
	//方向チェック
	if (CKFMath::Vec2Dot(vLineL, vDirL) < 0.0f) { return false; }

	//長さチェック
	if (CKFMath::VecMagnitudeSquare(vDirL) > CKFMath::VecMagnitudeSquare(vLineL)) { return false; }

	//LineR
	auto vDirR = vOut - vRA;
	//方向チェック
	if (CKFMath::Vec2Dot(vLineR, vDirR) < 0.0f) { return false; }

	//長さチェック
	if (CKFMath::VecMagnitudeSquare(vDirR) > CKFMath::VecMagnitudeSquare(vLineR)) { return false; }

	return true;
}

//--------------------------------------------------------------------------------
//	関数名：CheckRayWithField
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkLineWithLine(const CKFVec3& vLA, const CKFVec3& vLB, const CKFVec3& vRA, const CKFVec3& vRB, CKFVec3& vOut)
{
	auto vLineL = vLA - vLB;
	auto vLineR = vRA - vRB;

	if ((vLineL * vLineR) == CKFVec3(0.0f))
	{//平行
		return false;
	}

	//XY平面の交点の算出
	CKFVec2 vOutXY;
	if (!checkLineWithLine(CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), vOutXY))
	{
		return false;
	}

	CKFMath::VecNormalize(vLineL);
	CKFMath::VecNormalize(vLineR);

	//相応のZ値を算出する
	auto fRateL = vLineL.m_fX != 0.0f ? (vOutXY.m_fX - vLA.m_fX) / vLineL.m_fX
		: vLineL.m_fY != 0.0f ? (vOutXY.m_fY - vLA.m_fY) / vLineL.m_fY
		: 0.0f;
	auto fRateR = vLineR.m_fX != 0.0f ? (vOutXY.m_fX - vRA.m_fX) / vLineR.m_fX
		: vLineR.m_fY != 0.0f ? (vOutXY.m_fY - vRA.m_fY) / vLineR.m_fY
		: 0.0f;
	auto fZL = vLA.m_fZ + fRateL * vLineL.m_fZ;
	auto fZR = vRA.m_fZ + fRateR * vLineR.m_fZ;
	if (fZL != fZR) { return false; }

	vOut = CKFVec3(vOutXY.m_fX, vOutXY.m_fY, fZL);
	return true;
}

//--------------------------------------------------------------------------------
//  Box(AABBとOBB)のハーフサイズをとある軸に投影して長さを算出する
//--------------------------------------------------------------------------------
float CCollisionDetector::transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis)
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
bool CCollisionDetector::checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis)
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

//--------------------------------------------------------------------------------
//  checkOverlapOnAxis
//	ボックス同士がとある軸に重ねてるかどうかをチェックする(AABB)
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR)
{
	bool bAnswer = vMinL.m_fX < vMaxR.m_fX && vMinR.m_fX < vMaxL.m_fX
		&& vMinL.m_fY < vMaxR.m_fY && vMinR.m_fY < vMaxL.m_fY;
	return bAnswer;
}

//--------------------------------------------------------------------------------
//  checkOverlapAABB
//	ボックス同士がとXYZ軸に重ねてるかどうかをチェックする(AABB)
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR)
{
	const CKFVec3& vPosL = aabbL.GetWorldPos();
	const CKFVec3& vHalfSizeL = aabbL.GetHalfSize();
	const CKFVec3& vPosR = aabbR.GetWorldPos();
	const CKFVec3& vHalfSizeR = aabbR.GetHalfSize();
	CKFVec3 vMinL = vPosL - vHalfSizeL;
	CKFVec3 vMaxL = vPosL + vHalfSizeL;
	CKFVec3 vMinR = vPosR - vHalfSizeR;
	CKFVec3 vMaxR = vPosR + vHalfSizeR;

	//AABB同士がxyz軸上に重ねてるかどうかをチェックする
	bool bAnswer = checkOverlapOnAxis(CKFVec2(vMinL.m_fY, vMinL.m_fZ), CKFVec2(vMaxL.m_fY, vMaxL.m_fZ), CKFVec2(vMinR.m_fY, vMinR.m_fZ), CKFVec2(vMaxR.m_fY, vMaxR.m_fZ))	//X軸
		&& checkOverlapOnAxis(CKFVec2(vMinL.m_fZ, vMinL.m_fX), CKFVec2(vMaxL.m_fZ, vMaxL.m_fX), CKFVec2(vMinR.m_fZ, vMinR.m_fX), CKFVec2(vMaxR.m_fZ, vMaxR.m_fX))			//Y軸
		&& checkOverlapOnAxis(CKFVec2(vMinL.m_fX, vMinL.m_fY), CKFVec2(vMaxL.m_fX, vMaxL.m_fY), CKFVec2(vMinR.m_fX, vMinR.m_fY), CKFVec2(vMaxR.m_fX, vMaxR.m_fY));			//Z軸

	return bAnswer;
}
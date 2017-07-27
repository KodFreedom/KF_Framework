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
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR)
{
	CKFVec3 vSLPos = sphereL.GetWorldPos();
	float fSLRadius = sphereL.GetRadius();
	CKFVec3 vSRPos = sphereR.GetWorldPos();
	float fSRRadius = sphereR.GetRadius();
	CKFVec3 vMidLine = vSLPos - vSRPos;
	float fDisMin = fSLRadius + fSRRadius;
	float fDisSqr = CKFMath::VecMagnitudeSquare(vMidLine);

	if (fDisSqr < fDisMin * fDisMin)
	{
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
}

//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollisionSystem::CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	CKFVec3 vSpherePos = sphere.GetWorldPos();
	float fSphereRadius = sphere.GetRadius();
	CFieldColliderComponent::INFO info = field.GetPointInfo(vSpherePos);

	if (info.bInTheField == false) { return; }

	if (info.fHeight >= vSpherePos.m_fY - fSphereRadius)
	{
		if (sphere.IsTrigger())
		{
			auto list = sphere.GetGameObject()->GetBehaviorComponent();
			for (auto itr = list.begin(); itr != list.end(); ++itr)
			{
				(*itr)->OnTrigger(sphere, field);
			}
			return;
		}

		//sphere.OnCollision(field);
		//field.OnCollision(sphere);

		CGameObject* pSphereObj = sphere.GetGameObject();

		//位置調節
		CKFVec3 vMovement = CKFVec3(0.0f, info.fHeight + fSphereRadius - vSpherePos.m_fY, 0.0f);
		pSphereObj->GetTransformComponent()->MovePosNext(vMovement);

		//回転調節
		//CKFVec3 vUpNext = CKFMath::LerpNormal(pSphereObj->GetUpNext(), info.vFaceNormal, 0.2f);
		//pSphereObj->RotByUp(vUpNext);

		//速度調節
		CRigidbodyComponent* pRB = sphere.GetGameObject()->GetRigidbodyComponent();
		if (pRB->GetType() == CRigidbodyComponent::RB_3D)
		{
			C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;
			p3DRB->SetOnGround(true);
			CKFVec3 vVelocity = p3DRB->GetVelocity();
			vVelocity.m_fY = 0.0f;
			p3DRB->SetVelocity(vVelocity);
		}
	}
}
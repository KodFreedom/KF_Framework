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
#include "KF_CollisionSystem.h"
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

	float fDisMin = fSLRadius + fSRRadius;
	float fDisSqr = CKFMath::VecMagnitudeSquare(vSLPos - vSRPos);

	if (fDisSqr < fDisMin * fDisMin)
	{
		if (sphereL.IsTrigger() || sphereR.IsTrigger())
		{//トリガーだったら物理処理しない
			auto list = sphereL.GetGameObject()->GetBehaviorComponent();
			for (auto itr = list.begin(); itr != list.end(); ++itr)
			{
				(*itr)->OnTrigger(sphereR);
			}

			list = sphereR.GetGameObject()->GetBehaviorComponent();
			for (auto itr = list.begin(); itr != list.end(); ++itr)
			{
				(*itr)->OnTrigger(sphereL);
			}
			return;
		}

		//sphereL.OnCollision(sphereR);
		//sphereR.OnCollision(sphereL);

		//臨時処理
		CKFVec3 vCenter = (vSLPos + vSRPos) * 0.5f;
		CKFVec3 vDir = vSLPos - vSRPos;
		CKFMath::VecNormalize(vDir);

		//L
		CRigidbodyComponent* pRB = sphereL.GetGameObject()->GetRigidbodyComponent();
		if (pRB->GetType() == CRigidbodyComponent::RB_3D)
		{
			sphereL.GetGameObject()->GetTransformComponent()->MovePosNext(vCenter + vDir * fSLRadius - vSLPos);
		}

		//R
		pRB = sphereR.GetGameObject()->GetRigidbodyComponent();
		if (pRB->GetType() == CRigidbodyComponent::RB_3D)
		{
			sphereR.GetGameObject()->GetTransformComponent()->MovePosNext(vCenter - vDir * fSRRadius - vSRPos);
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
				(*itr)->OnTrigger(field);
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
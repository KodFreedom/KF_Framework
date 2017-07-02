//--------------------------------------------------------------------------------
//	コリジョン判定クラス
//　KF_Collision.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_Collision.h"
#include "KF_Math.h"
#include "gameObject.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CKFCollision::CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	CKFVec3 vSpherePos = sphere.GetGameObject()->GetPosNext();
	float fSphereRadius = sphere.GetRadius();
	CFieldColliderComponent::INFO info = field.GetPointInfo(vSpherePos);

	if (info.bInTheField == false) { return; }

	if (info.fHeight >= vSpherePos.m_fY - fSphereRadius)
	{
		if (sphere.IsTrigger())
		{
			sphere.OnTrigger(field);
			return;
		}

		sphere.OnCollision(field);
		field.OnCollision(sphere);

		CGameObject* pSphereObj = sphere.GetGameObject();

		//位置調節
		vSpherePos.m_fY = info.fHeight + fSphereRadius;
		pSphereObj->SetPosNext(vSpherePos);

		//回転調節
		CKFVec3 vUpNext = CKFMath::LerpNormal(pSphereObj->GetUpNext(), info.vFaceNormal, 0.2f);
		pSphereObj->RotByUp(vUpNext);

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
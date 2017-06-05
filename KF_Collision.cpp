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
void CKFCollision::CheckContactSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	CKFVec3 vSpherePos = sphere.GetGameObject()->GetPosNext();
	float fSphereRadius = sphere.GetRadius();
	float fFieldHeight = field.GetHeight(vSpherePos);

	if (fFieldHeight >= vSpherePos.m_fY - fSphereRadius)
	{
		vSpherePos.m_fY = fFieldHeight + fSphereRadius;
		sphere.GetGameObject()->SetPosNext(vSpherePos);

		CRigidbodyComponent* pRB = sphere.GetGameObject()->GetRigidbodyComponent();
		if (pRB->GetType() == CRigidbodyComponent::RB_3D)
		{
			C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;
			CKFVec3 vVelocity = p3DRB->GetVelocity();
			vVelocity.m_fY = 0.0f;
			p3DRB->SetVelocity(vVelocity);
		}
	}
}
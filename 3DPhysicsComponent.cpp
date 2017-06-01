//--------------------------------------------------------------------------------
//
//　3DPhysicsComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DPhysicsComponent.h"
#include "gameObject3D.h"
#include "manager.h"
#include "modeDemo.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
C3DPhysicsComponent::C3DPhysicsComponent() : CPhysicsComponent()
	, m_fMovement(0.0f)
	, m_fRot(0.0f)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DPhysicsComponent::Update(CGameObject &gameObj)
{
	CGameObject3D* pObj = (CGameObject3D*)&gameObj;
	CCamera* pCamera = GetManager()->GetModeNow()->GetCamera();

	CKFVec3 vPos = pObj->GetPosNext();
	CKFVec3 vRot = pObj->GetRotNext();
	CKFVec3 vForwardNext = pObj->GetForwardNext();
	CKFVec3 vForward = pCamera->GetVecLook();
	CKFVec3 vUp = pObj->GetUpNext();

	//処理開始
	vForward.m_fY = 0.0f;
	CKFMath::VecNormalize(&vForward);

	//向き算出
	if (m_fRot != 0.0f)
	{
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, m_fRot);
		CKFMath::Vec3TransformNormal(&vForward, mtxYaw);
	}

	vPos += vForward * m_fMovement;
	//ここでMeshfieldと当たり判定
	vPos.m_fY = ((CModeDemo*)GetManager()->GetModeNow())->GetHeight(vPos);

	//回転計算
	float fRotChanged = CKFMath::RadianBetweenVec3(vForward, vForwardNext);
	fRotChanged *= 0.2f;
	if (fRotChanged != 0.0f)
	{
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, fRotChanged);
		CKFMath::Vec3TransformNormal(&vForwardNext, mtxYaw);
	}
	vRot.m_fY += fRotChanged;
	CKFMath::NormalizeRotInTwoPi(&vRot);

	//処理完了
	m_fMovement = 0.0f;
	m_fRot = 0.0f;
	pObj->SetPosNext(vPos);
	pObj->SetRotNext(vRot);
	pObj->SetForwardNext(vForwardNext);
	pObj->SetUpNext(vUp);
}
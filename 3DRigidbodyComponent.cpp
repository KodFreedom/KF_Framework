//--------------------------------------------------------------------------------
//
//　3DRigidbodyComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DRigidbodyComponent.h"
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
C3DRigidbodyComponent::C3DRigidbodyComponent(CGameObject* const pGameObj)
	: CRigidbodyComponent(pGameObj, RB_3D)
	, m_fMass(1.0f)
	, m_fInverseMass(1.0f)
	, m_fDrag(0.95f)
	, m_vGravity(CKFVec3(0.0f, -0.098f, 0.0f))
	, m_vMovement(CKFVec3(0.0f))
	, m_vVelocity(CKFVec3(0.0f))
	, m_vForceAccum(CKFVec3(0.0f))
	, m_vAcceleration(CKFVec3(0.0f))
	, m_bOnGround(false)
{
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::SetMass(const float& fMass)
{
	if (fMass <= 0.0f) { return; }
	m_fMass = fMass;
	m_fInverseMass = 1.0f / fMass;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::Update(void)
{
	CGameObject3D* pObj = (CGameObject3D*)m_pGameObj;
	CKFVec3 vPos = pObj->GetPosNext();

	//重力
	float f = DELTA_TIME;
	m_vForceAccum += m_vGravity * m_fMass * DELTA_TIME;

	//速度
	m_vVelocity += m_vForceAccum * m_fInverseMass;

	//位置更新
	vPos += m_vVelocity;
	vPos += m_vMovement;

	//処理完了
	m_vForceAccum = CKFVec3(0.0f);
	m_vVelocity *= m_fDrag;
	m_vMovement = CKFVec3(0.0f);
	pObj->SetPosNext(vPos);
}
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
	, m_fDrag(0.98f)
	, m_fAngularDrag(1.0f)
	, m_fFriction(1.0f)
	, m_fBounciness(0.0f)
	, m_vGravity(CKFVec3(0.0f, -0.5f, 0.0f))
	, m_vMovement(CKFVec3(0.0f))
	, m_vVelocity(CKFVec3(0.0f))
	, m_vForceAccum(CKFVec3(0.0f))
	, m_bOnGround(false)
	, m_bRotLock(0)
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
	//重力
	m_vForceAccum += m_vGravity * m_fMass * DELTA_TIME;

	//力から加速度を計算する
	CKFVec3 vAcceleration = m_vForceAccum * m_fInverseMass;
	
	//回転力から回転加速度を計算する
	CKFVec3 vAngularAcceleration = CKFMath::Vec3TransformCoord(m_vTorqueAccum, m_mtxInverseInertisTensor);

	//速度
	m_vVelocity += vAcceleration;
	m_vAngularVelocity += vAngularAcceleration;

	//位置更新
	CTransformComponent* pTrans = m_pGameObj->GetTransformComponent();
	pTrans->MovePosNext(m_vVelocity);
	pTrans->MovePosNext(m_vMovement);

	//回転更新
	pTrans->RotByEuler(m_vAngularVelocity);

	//処理完了
	m_vForceAccum = CKFVec3(0.0f);
	m_vTorqueAccum = CKFVec3(0.0f);
	m_vVelocity *= m_fDrag;
	m_vAngularVelocity *= m_fAngularDrag;
	m_vMovement = CKFVec3(0.0f);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DRigidbodyComponent::LateUpdate(void)
{
	//位置更新
	m_pGameObj->GetTransformComponent()->MovePosNext(m_vMovement);

	//処理完了
	m_vMovement = CKFVec3(0.0f);
}
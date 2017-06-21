//--------------------------------------------------------------------------------
//	アクションゲーム用カメラクラス
//　actionGameCamera.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "actionGameCamera.h"
#include "manager.h"
#include "rendererDX.h"
#include "inputManager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float		CActionGameCamera::sc_fRotSpeed = 0.05f;
const float		CActionGameCamera::sc_fStartRotMin = 0.2f;
const float		CActionGameCamera::sc_fRotLerpTime = 0.1f;
const float		CActionGameCamera::sc_fZoomSpeed = 0.25f;
const float		CActionGameCamera::sc_fDistanceMin = 3.0f;
const float		CActionGameCamera::sc_fDistanceMax = 10.0f;
const float		CActionGameCamera::sc_fZoomLerpTime = 0.2f;
const float		CActionGameCamera::sc_fMoveLerpTime = 0.075f;
const float		CActionGameCamera::sc_fReRotLerpTime = 0.025f;
const float		CActionGameCamera::sc_fDisToTarget = 3.0f;
const float		CActionGameCamera::sc_fRotXMax = 65.0f / 180.0f * KF_PI;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CActionGameCamera::CActionGameCamera() : CCamera()
	, m_vRotSpeed(CKFVec3(0.0f))
	, m_fZoomSpeed(0.0f)
{
	CNullGameObject nullGameObj;
	m_pTarget = &nullGameObj;

	//最大角度設定
	m_vLimitLook = CKFVec3(0.0f, -tanf(sc_fRotXMax), 1.0f);
	CKFMath::VecNormalize(&m_vLimitLook);

	//デフォルト角度設定
	m_vDefaultLook = CKFVec3(0.0f, -tanf(30.0f / 180.0f * KF_PI), 1.0f);
	CKFMath::VecNormalize(&m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CActionGameCamera::Update(void)
{
	CInputManager *pInput = GetManager()->GetInputManager();
	CKFVec3 vRot = CKFVec3(0.0f);
	float fZoomSpeed = 0.0f;

	float fRAxisX = pInput->GetRotHorizontal();
	float fRAxisY = pInput->GetRotVertical();
	float fZoom = pInput->GetZoom();

	//注目点回転
	if (fabsf(fRAxisX) > sc_fStartRotMin)
	{//Y軸回転
		vRot.m_fY = sc_fRotSpeed * fRAxisX;
	}
	if (fabsf(fRAxisY) > sc_fStartRotMin)
	{//X軸回転
		vRot.m_fX = sc_fRotSpeed * fRAxisY;
	}

	//拡大縮小
	if (fabsf(fZoom) > sc_fStartRotMin)
	{
		fZoomSpeed = sc_fZoomSpeed * fZoom;
	}

	//回転
	m_vRotSpeed = CKFMath::LerpVec3(m_vRotSpeed, vRot, sc_fRotLerpTime);
	Yaw(m_vRotSpeed.m_fY);
	Pitch(m_vRotSpeed.m_fX);

	//ズーム
	m_fZoomSpeed = CKFMath::LerpFloat(m_fZoomSpeed, fZoomSpeed, sc_fZoomLerpTime);
	m_fDistance += m_fZoomSpeed;
	m_fDistance = m_fDistance < sc_fDistanceMin ? sc_fDistanceMin : m_fDistance > sc_fDistanceMax ? sc_fDistanceMax : m_fDistance;

	//Target移動したらカメラの角度をデフォルトの角度にだんだん修正しに行く
	if (m_pTarget->GetPos() != m_pTarget->GetPosNext())
	{
		m_vVecLook = CKFMath::LerpNormal(m_vVecLook, m_vDefaultLook, sc_fReRotLerpTime);
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CActionGameCamera::LateUpdate(void)
{
	//カメラ移動
	m_vPosAt = CKFMath::LerpVec3(m_vPosAt, m_pTarget->GetPos(), sc_fMoveLerpTime);
}

//--------------------------------------------------------------------------------
//  Y軸回転
//--------------------------------------------------------------------------------
void CActionGameCamera::Yaw(const float& fAngle)
{
	CCamera::Yaw(fAngle);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  カメラのノーマライズ
//--------------------------------------------------------------------------------
void CActionGameCamera::NormalizeCamera(void)
{
	CCamera::NormalizeCamera();
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  カメラの回転する
//--------------------------------------------------------------------------------
void CActionGameCamera::ReCalcDefault(void)
{
	m_vDefaultLook = m_vVecRight * (m_vDefaultLook * m_vVecRight);
	CKFMath::VecNormalize(&m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  X軸回転角度算出
//--------------------------------------------------------------------------------
float CActionGameCamera::CalcRotX(void)
{
	CKFVec2 vX = CKFVec2(m_vVecLook.m_fX, m_vVecLook.m_fZ);
	float fRot = atan2f(-m_vVecLook.m_fY, CKFMath::VecMagnitude(vX));
	return fRot;
}
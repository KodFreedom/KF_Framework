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
const float	CActionGameCamera::sc_fRotSpeed = 0.05f;
const float	CActionGameCamera::sc_fStartRotMin = 0.2f;
const float	CActionGameCamera::sc_fRotLerpTime = 0.1f;
const float	CActionGameCamera::sc_fZoomSpeed = 0.25f;
const float	CActionGameCamera::sc_fDistanceMin = 3.0f;
const float	CActionGameCamera::sc_fDistanceDef = 6.0f;
const float	CActionGameCamera::sc_fDistanceMax = 10.0f;
const float	CActionGameCamera::sc_fZoomLerpTime = 0.2f;
const float	CActionGameCamera::sc_fMoveLerpTime = 0.075f;
const float	CActionGameCamera::sc_fReRotLerpTime = 0.025f;
const float	CActionGameCamera::sc_fRotXMax = 60.0f / 180.0f * KF_PI;
const float	CActionGameCamera::sc_fRotXDefault = 15.0f / 180.0f * KF_PI;
const float	CActionGameCamera::sc_fCollisionRadius = 0.1f;
const float CActionGameCamera::sc_fPosAtHeight = 1.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CActionGameCamera::CActionGameCamera() : CCamera()
	, m_vRotSpeed(Vector3(0.0f))
	, ZoomSpeed(0.0f)
	, PositionAtNext(Vector3(0.0f))
	, PositionEyeNext(Vector3(0.0f))
	, m_vVecLookNext(Vector3(0.0f))
	, m_vVecUpNext(Vector3(0.0f))
	, m_vVecRightNext(Vector3(0.0f))
	, DistanceNext(0.0f)
	, m_vDefaultLook(Vector3(0.0f))
	, m_pTarget(nullptr)
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CActionGameCamera::Init(void)
{
	CCamera::Init();

	//ダブルバッファ初期値設定
	PositionAtNext = PositionAt;
	PositionEyeNext = PositionEye;
	m_vVecLookNext = m_vVecLook;
	m_vVecRightNext = m_vVecRight;
	m_vVecUpNext = m_vVecUp;
	DistanceNext = Distance;

	//デフォルト角度設定
	m_vDefaultLook = Vector3(0.0f, -tanf(sc_fRotXDefault), 1.0f);
	CKFMath::VecNormalize(m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CActionGameCamera::Update(void)
{
	SwitchParam();

	Input *pInput = Main::GetManager()->GetInputManager();
	Vector3 vRot = Vector3(0.0f);
	float fZoomSpeed = 0.0f;

	float fRAxisX = pInput->GetRotHorizontal();
	float fRAxisY = pInput->GetRotVertical();
	float fZoom = pInput->GetZoom();

	//注目点回転
	if (fabsf(fRAxisX) > sc_fStartRotMin)
	{//Y軸回転
		vRot.Y = sc_fRotSpeed * fRAxisX;
	}
	if (fabsf(fRAxisY) > sc_fStartRotMin)
	{//X軸回転
		vRot.Y = sc_fRotSpeed * fRAxisY;
	}

	//拡大縮小
	if (fabsf(fZoom) > sc_fStartRotMin)
	{
		fZoomSpeed = sc_fZoomSpeed * fZoom;
	}

	//回転速度算出
	m_vRotSpeed = CKFMath::LerpVec3(m_vRotSpeed, vRot, sc_fRotLerpTime);
	
	//回転を制限する
	LimitRot();

	//回転
	Yaw(m_vRotSpeed.Y);
	Pitch(m_vRotSpeed.Y);

	//Target移動したらカメラの角度をデフォルトの角度にだんだん修正しに行く
	//if (m_pTarget && m_pTarget->GetTransformComponent()->GetPos() != m_pTarget->GetTransformComponent()->GetPosNext())
	//{
	//	m_vVecLookNext = CKFMath::LerpNormal(m_vVecLookNext, m_vDefaultLook, sc_fReRotLerpTime);
	//}

	//世界との衝突判定
	//CheckCollision();



	//ズーム
	/*ZoomSpeed = CKFMath::LerpFloat(ZoomSpeed, fZoomSpeed, sc_fZoomLerpTime);
	Distance += ZoomSpeed;
	Distance = Distance < sc_fDistanceMin ? sc_fDistanceMin : Distance > sc_fDistanceMax ? sc_fDistanceMax : Distance;*/

	//if (!Main::GetManager()->GetColliderManager()->CheckActionCameraCollision(this))
	//{
	//	Distance = CKFMath::LerpFloat(Distance, sc_fDistanceDef, sc_fZoomLerpTime);
	//}

	
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CActionGameCamera::LateUpdate(void)
{
	//カメラ移動
	if (m_pTarget)
	{
		PositionAtNext = CKFMath::LerpVec3(PositionAtNext, m_pTarget->GetTransformComponent()->GetPos() + Vector3(0.0f, 1.0f, 0.0f) * sc_fPosAtHeight, sc_fMoveLerpTime);
	}

	//カメラノーマライズ
	NormalizeCamera();
	PositionEyeNext = PositionAtNext - m_vVecLookNext * DistanceNext;
}

//--------------------------------------------------------------------------------
//  Y軸回転
//--------------------------------------------------------------------------------
void CActionGameCamera::Yaw(const float& fAngle)
{
	Matrix44 mtxYaw;
	Vector3 vAxisY = Vector3(0.0f, 1.0f, 0.0f);
	CKFMath::MtxRotAxis(mtxYaw, vAxisY, fAngle);
	m_vVecRightNext = CKFMath::Vec3TransformNormal(m_vVecRightNext, mtxYaw);
	m_vVecLookNext = CKFMath::Vec3TransformNormal(m_vVecLookNext, mtxYaw);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  X軸回転
//--------------------------------------------------------------------------------
void CActionGameCamera::Pitch(const float& fAngle)
{
	Matrix44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vVecRightNext, fAngle);
	m_vVecUpNext = CKFMath::Vec3TransformNormal(m_vVecUpNext, mtxPitch);
	m_vVecLookNext = CKFMath::Vec3TransformNormal(m_vVecLookNext, mtxPitch);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  カメラのノーマライズ
//--------------------------------------------------------------------------------
void CActionGameCamera::NormalizeCamera(void)
{
	CKFMath::VecNormalize(m_vVecLookNext);
	m_vVecUpNext = m_vVecLookNext * m_vVecRightNext;
	CKFMath::VecNormalize(m_vVecUpNext);
	m_vVecRightNext = m_vVecUpNext * m_vVecLookNext;
	CKFMath::VecNormalize(m_vVecRightNext);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  カメラの回転する
//--------------------------------------------------------------------------------
void CActionGameCamera::ReCalcDefault(void)
{
	m_vDefaultLook = m_vVecRightNext * (m_vDefaultLook * m_vVecRightNext);
	CKFMath::VecNormalize(m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  X軸回転角度算出
//--------------------------------------------------------------------------------
float CActionGameCamera::CalcRotX(void)
{
	Vector2 vX = Vector2(m_vVecLook.Y, m_vVecLook.Z);
	float fRot = atan2f(-m_vVecLook.Y, CKFMath::VecMagnitude(vX));
	return fRot;
}

//--------------------------------------------------------------------------------
//  カメラ回転を制限する
//--------------------------------------------------------------------------------
void CActionGameCamera::LimitRot(void)
{
	//X軸回転制限
	float fRotXNow = CalcRotX();
	if (m_vRotSpeed.Y + fRotXNow >= sc_fRotXMax)
	{
		m_vRotSpeed.Y = sc_fRotXMax - fRotXNow;
	}
	else if (m_vRotSpeed.Y + fRotXNow <= -sc_fRotXMax)
	{
		m_vRotSpeed.Y = -sc_fRotXMax - fRotXNow;
	}
}

//--------------------------------------------------------------------------------
//  変数交換
//--------------------------------------------------------------------------------
void CActionGameCamera::SwitchParam(void)
{
	PositionAt = PositionAtNext;
	PositionEye = PositionEyeNext;
	m_vVecLook = m_vVecLookNext;
	m_vVecUp = m_vVecUpNext;
	m_vVecRight = m_vVecRightNext;
}

//--------------------------------------------------------------------------------
//  マップとのコリジョンチェック
//--------------------------------------------------------------------------------
void CActionGameCamera::CheckCollision(void)
{
	//CColliderManager::HIT_INFO hitInfo;
	//PositionEyeNext = PositionAtNext - m_vVecLookNext * DistanceNext;

	//if (Main::GetManager()->GetColliderManager()->SphereCast(PositionEyeNext, sc_fCollisionRadius, hitInfo))
	//{//衝突したら
	//	//新しいカメラの算出-
	//	Vector3 vNewPosEye = hitInfo.Position + hitInfo.Normal * sc_fCollisionRadius;
	//	
	//	//新しい前方向の算出
	//	m_vVecLookNext = PositionAtNext - vNewPosEye;

	//	//新しい距離の算出
	//	DistanceNext = CKFMath::VecMagnitude(m_vVecLookNext);

	//	//if (m_vVecLookNext.Y != 0.0f)
	//	//{
	//	//	fNewDis = fabsf((vNewPos.Y - PositionAtNext.Y) / m_vVecLookNext.Y);
	//	//}
	//	//DistanceNext = CKFMath::LerpFloat(DistanceNext, fNewDis, sc_fZoomLerpTime);
	//}
	//else
	//{
	//	DistanceNext = CKFMath::LerpFloat(DistanceNext, sc_fDistanceDef, sc_fZoomLerpTime);
	//}
}
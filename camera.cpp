//--------------------------------------------------------------------------------
//	カメラクラス
//　Camera.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "camera.h"
#include "manager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
//#define CAMERA_MOVE_SPEED (1.0f)
//#define RETURN_SPEED (0.95f)
//#define FLOAT_MIN (0.001f)
//#define WORLD_ROT_DEFAULT (CKFVec3(0.0f, 0.0f, 0.0f))
//#define CAMERA_FOV_DEFAULT (75.0f * D3DX_PI / 180.0f)
//#define CAMERA_FOV_MIN (0.1f)
//#define CAMERA_FOV_MAX (D3DX_PI - 0.1f)
//#define CAMERA_MOVE_RATE (0.1f)

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CCamera::CCamera()
	: m_vMovement(CKFVec3(0.0f))
	, m_vPosAt(CKFVec3(0.0f))
	, m_vPosEye(CKFVec3(0.0f, 5.0f, -5.0f))
	, m_vVecLook(CKFVec3(0.0f))
	, m_vVecUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vVecRight(CKFVec3(1.0f, 0.0f, 0.0f))
	, m_fDistance(0.0f)
	, m_fFovY((float)DEFAULT_FOV)
	, m_fFar((float)DEFAULT_FAR)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CCamera::Init(void)
{
	m_vVecLook = m_vPosAt - m_vPosEye;
	CKFMath::VecNormalize(m_vVecLook);
	m_fDistance = CKFMath::VecMagnitude(m_vPosEye - m_vPosAt);
	CCamera::NormalizeCamera();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CCamera::Update(void)
{
//	CMouseDX *pMouse = CMain::GetManager()->GetMouse();
//	CJoystickDX* pJoystick = CMain::GetManager()->GetJoystickDX();
//	CKFVec3 vRot = CKFVec3(0.0f);
//	float fZoomSpeed = 0.0f;
//
//	//注目点回転
//	if (pMouse->GetMousePress(CMouseDX::MOUSE_RIGHT) && pMouse->GetMouseAxisX() != 0)
//	{
//		vRot.m_fY = KF_PI * ((float)pMouse->GetMouseAxisX() / SCREEN_WIDTH);
//	}
//	if (pMouse->GetMousePress(CMouseDX::MOUSE_RIGHT) && pMouse->GetMouseAxisY() != 0)
//	{
//		vRot.m_fX = KF_PI * ((float)pMouse->GetMouseAxisY() / SCREEN_HEIGHT);
//	}
//
//	//拡大縮小
//	if (pMouse->GetMouseAxisZ() != 0 /*&& pMouse->GetMouseAxisZ() <= MOUSE_AXIS_MAX && pMouse->GetMouseAxisZ() >= -MOUSE_AXIS_MAX*/)
//	{
//		fZoomSpeed = -sc_fZoomSpeed * (float)pMouse->GetMouseAxisZ() / MOUSE_AXIS_MAX;
//	}
//
//	if (pJoystick->GetAttached())
//	{
//		float fRAxisX = (float)pJoystick->GetRStickAxisX() / CJoystickDX::sc_nStickAxisMax;
//		float fRAxisY = (float)pJoystick->GetRStickAxisY() / CJoystickDX::sc_nStickAxisMax;
//		float fLTRT = (float)pJoystick->GetLTandRT() / CJoystickDX::sc_nStickAxisMax;
//
//#ifdef _DEBUG
//			//char aBuf[256] = {};
//			//sprintf(aBuf, "AxisX : %f\tAxisY : %f\n", fRAxisX, fRAxisY);
//			//OutputDebugString(aBuf);
//#endif
//		//注目点回転
//		if (fabsf(fRAxisX) > sc_fStartRotMin)
//		{//Y軸回転
//			vRot.m_fY = sc_fRotSpeed * fRAxisX;
//		}
//		if (fabsf(fRAxisY) > sc_fStartRotMin)
//		{//X軸回転
//			vRot.m_fX = sc_fRotSpeed * fRAxisY;
//		}
//
//		//拡大縮小
//		if (fabsf(fLTRT) > sc_fStartRotMin)
//		{
//			fZoomSpeed = sc_fZoomSpeed * fLTRT;
//		}
//	}
//
//	//注目点回転
//	m_vRotSpeed = CKFMath::LerpVec3(m_vRotSpeed, vRot, sc_fRotLerpTime);
//	Yaw(m_vRotSpeed.m_fY);
//	Pitch(m_vRotSpeed.m_fX);
//
//	//ズーム
//	m_fZoomSpeed = CKFMath::LerpFloat(m_fZoomSpeed, fZoomSpeed, sc_fZoomLerpTime);
//	m_fDistance += m_fZoomSpeed;
//	m_fDistance = m_fDistance < sc_fDistanceMin ? sc_fDistanceMin : m_fDistance > sc_fDistanceMax ? sc_fDistanceMax : m_fDistance;
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CCamera::LateUpdate(void)
{
	NormalizeCamera();
	m_vPosEye = m_vPosAt - m_vVecLook * m_fDistance;
}

//--------------------------------------------------------------------------------
//  セット処理(描画直前)
//--------------------------------------------------------------------------------
void CCamera::Set(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	//View行列
	D3DXMATRIX mtxView;
	D3DXMatrixLookAtLH(&mtxView,& (D3DXVECTOR3)m_vPosEye,& (D3DXVECTOR3)m_vPosAt,& (D3DXVECTOR3)m_vVecUp);//左手座標系
	pDevice->SetTransform(D3DTS_VIEW,&mtxView);

	//プロジェクション行列
	D3DXMATRIX mtxProjection;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		m_fFovY * KF_PI / 180.0f,//視野角度(半分)
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,//アスペクト比
		0.01f,//near 0.0fより大きい値
		m_fFar);//far nearより大きい値
	pDevice->SetTransform(D3DTS_PROJECTION,& mtxProjection);

	//逆view
	UpdateViewInverse(mtxView);
#endif
}

//--------------------------------------------------------------------------------
//  View行列の逆行列を計算する
//--------------------------------------------------------------------------------
void CCamera::UpdateViewInverse(const D3DXMATRIX& mtxView)
{
	D3DXMatrixInverse(&m_mtxViewInverse, NULL,& mtxView);

	//移動量をカットする
	//m_mtxViewInverse._41 = 0.0f;
	//m_mtxViewInverse._42 = 0.0f;
	//m_mtxViewInverse._43 = 0.0f;
}

//--------------------------------------------------------------------------------
//  View行列の逆行列を取得する
//--------------------------------------------------------------------------------
D3DXMATRIX CCamera::GetMtxViewInverse(void)
{
	return m_mtxViewInverse;
}

//--------------------------------------------------------------------------------
//  移動処理
//--------------------------------------------------------------------------------
void CCamera::MoveCamera(const CKFVec3& vMovement)
{
	m_vPosEye += vMovement;
	m_vPosAt += vMovement;
}

//--------------------------------------------------------------------------------
//  移動処理
//--------------------------------------------------------------------------------
void CCamera::LookAtHere(const CKFVec3& vPos)
{
	m_vMovement = vPos - m_vPosAt;
}

//--------------------------------------------------------------------------------
//  前向き取得
//--------------------------------------------------------------------------------
CKFVec3 CCamera::GetVecLook(void)
{
	return m_vVecLook;
}

//--------------------------------------------------------------------------------
//  前向き取得
//--------------------------------------------------------------------------------
CKFVec3 CCamera::GetVecUp(void)
{
	return m_vVecUp;
}

//--------------------------------------------------------------------------------
//  前向き取得
//--------------------------------------------------------------------------------
CKFVec3 CCamera::GetVecRight(void)
{
	return m_vVecRight;
}

//--------------------------------------------------------------------------------
//  前向き取得
//--------------------------------------------------------------------------------
CKFVec3 CCamera::GetPosAt(void)
{
	return m_vPosAt;
}

//--------------------------------------------------------------------------------
//  前向き取得
//--------------------------------------------------------------------------------
CKFVec3 CCamera::GetPosEye(void)
{
	return m_vPosEye;
}

//--------------------------------------------------------------------------------
//  カメラ設定
//--------------------------------------------------------------------------------
void CCamera::SetCamera(const CKFVec3& vPosAt, const CKFVec3& vPosEye, const CKFVec3& vUp, const CKFVec3& vRight)
{
	m_vPosAt = vPosAt;
	m_vPosEye = vPosEye;
	m_vVecUp = vUp;
	m_vVecRight = vRight;
	m_vVecLook = m_vPosAt - m_vPosEye;
	m_fDistance = CKFMath::VecMagnitude((m_vPosEye - m_vPosAt));
	NormalizeCamera();
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  カメラ前方と上と右を常に90度になってるを確保する
//--------------------------------------------------------------------------------
void CCamera::NormalizeCamera(void)
{
	CKFMath::VecNormalize(m_vVecLook);
	m_vVecUp = m_vVecLook * m_vVecRight;
	CKFMath::VecNormalize(m_vVecUp);
	m_vVecRight = m_vVecUp * m_vVecLook;
	CKFMath::VecNormalize(m_vVecRight);
}

//--------------------------------------------------------------------------------
//  X軸(vVecRight)回転
//--------------------------------------------------------------------------------
void CCamera::Pitch(const float& fAngle)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vVecRight, fAngle);
	m_vVecUp = CKFMath::Vec3TransformNormal(m_vVecUp, mtxPitch);
	m_vVecLook = CKFMath::Vec3TransformNormal(m_vVecLook, mtxPitch);
}

//--------------------------------------------------------------------------------
//  Y軸(vVecUp)回転
//--------------------------------------------------------------------------------
void CCamera::Yaw(const float& fAngle)
{
	CKFMtx44 mtxYaw;
	CKFVec3 vAxisY = CKFVec3(0.0f, 1.0f, 0.0f);
	CKFMath::MtxRotAxis(mtxYaw, vAxisY, fAngle);
	m_vVecRight = CKFMath::Vec3TransformNormal(m_vVecRight, mtxYaw);
	m_vVecLook = CKFMath::Vec3TransformNormal(m_vVecLook, mtxYaw);
}

//--------------------------------------------------------------------------------
//  Z軸(vLook)回転
//--------------------------------------------------------------------------------
void CCamera::Roll(const float& fAngle)
{
	CKFMtx44 mtxRoll;
	CKFMath::MtxRotAxis(mtxRoll, m_vVecLook, fAngle);
	m_vVecUp = CKFMath::Vec3TransformNormal(m_vVecUp, mtxRoll);
	m_vVecRight = CKFMath::Vec3TransformNormal(m_vVecRight, mtxRoll);
}
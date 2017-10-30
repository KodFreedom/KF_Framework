//--------------------------------------------------------------------------------
//	エディタ用カメラクラス
//　actionGameCamera.h
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "editorCamera.h"
#include "inputDX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float	CEditorCamera::sc_fRotSpeed = 0.05f;
const float	CEditorCamera::sc_fStartRotMin = 0.2f;
const float	CEditorCamera::sc_fZoomSpeed = 0.5f;
const float	CEditorCamera::sc_fDistanceMin = 3.0f;
const float	CEditorCamera::sc_fDistanceMax = 500.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CEditorCamera::CEditorCamera() : CCamera()
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CEditorCamera::Init(void)
{
	CCamera::Init();
	SetCamera(Vector3(0.0f), Vector3(0.0f, 20.0f, -20.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CEditorCamera::Update(void)
{
	auto pInput = CMain::GetManager()->GetInputManager();
	if (!pInput->GetMouse()->GetMousePress(CMouseDX::MOUSE_RIGHT)) { return; }
	
	Vector3 vRot;
	float fZoomSpeed = 0.0f;
	float fAxisX = pInput->GetMoveHorizontal();
	float fAxisY = pInput->GetMoveVertical();
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

	//回転
	Yaw(vRot.Y);
	Pitch(vRot.Y);

	//ズーム
	m_fDistance += fZoomSpeed;
	m_fDistance = m_fDistance < sc_fDistanceMin ? sc_fDistanceMin : m_fDistance > sc_fDistanceMax ? sc_fDistanceMax : m_fDistance;
}
#endif // _DEBUG
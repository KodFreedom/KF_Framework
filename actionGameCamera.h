//--------------------------------------------------------------------------------
//	アクションゲーム用カメラクラス
//　actionGameCamera.h
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "camera.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CActionGameCamera : public CCamera
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CActionGameCamera();
	~CActionGameCamera() {}
	
	void Update(void) override;
	void LateUpdate(void) override;

	//Set関数
	void SetTarget(CGameObject* pTarget) override { m_pTarget = pTarget; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const float		sc_fRotSpeed;		//カメラ回転速度
	static const float		sc_fStartRotMin;	//カメラ回転開始のスティック最小値
	static const float		sc_fRotLerpTime;	//カメラ回転速度遅延処理係数
	static const float		sc_fZoomSpeed;		//ズーム速度
	static const float		sc_fDistanceMin;	//注視点と注目点の最小距離
	static const float		sc_fDistanceMax;	//注視点と注目点の最大距離
	static const float		sc_fZoomLerpTime;	//ズーム遅延処理係数
	static const float		sc_fMoveLerpTime;	//移動遅延処理係数
	static const float		sc_fReRotLerpTime;	//角度戻す遅延処理係数	
	static const float		sc_fDisToTarget;	//注視点とターゲットとの距離
	static const float		sc_fRotXMax;		//X軸回転の最大角度

	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	void	Yaw(const float& fAngle) override;
	void	NormalizeCamera(void) override;
	void	ReCalcDefault(void);
	float	CalcRotX(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CGameObject*	m_pTarget;			//カメラが見てるターゲット
	CKFVec3			m_vLimitLook;		//最大角度の前方向
	CKFVec3			m_vDefaultLook;		//デフォルト前方向
	CKFVec3			m_vRotSpeed;		//X軸回転速度
	float			m_fZoomSpeed;		//ズーム速度	
};
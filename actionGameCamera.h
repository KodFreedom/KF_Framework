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
	
	void Init(void) override;
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
	static const float		sc_fDistanceDef;	//注視点と注目点のデフォルト値
	static const float		sc_fDistanceMax;	//注視点と注目点の最大距離
	static const float		sc_fZoomLerpTime;	//ズーム遅延処理係数
	static const float		sc_fMoveLerpTime;	//移動遅延処理係数
	static const float		sc_fReRotLerpTime;	//角度戻す遅延処理係数	
	static const float		sc_fRotXMax;		//X軸回転の最大角度
	static const float		sc_fRotXDefault;	//X軸回転のデフォルト値
	static const float		sc_fCollisionRadius;//当たり判定用の半径
	static const float		sc_fPosAtHeight;	//注視点の高さ

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	Yaw(const float& fAngle) override;
	void	Pitch(const float& fAngle) override;
	void	NormalizeCamera(void) override;
	void	ReCalcDefault(void);
	float	CalcRotX(void);
	void	LimitRot(void);
	void	SwitchParam(void);
	void	CheckCollision(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CKFVec3			m_vPosAtNext;		//注視点の位置
	CKFVec3			m_vPosEyeNext;		//カメラの位置
	CKFVec3			m_vVecLookNext;		//カメラの前方向情報
	CKFVec3			m_vVecUpNext;		//カメラの上方向情報
	CKFVec3			m_vVecRightNext;	//カメラの右方向情報
	float			m_fDistanceNext;	//AtとEyeの距離

	CGameObject*	m_pTarget;			//カメラが見てるターゲット
	CKFVec3			m_vDefaultLook;		//デフォルト前方向
	CKFVec3			m_vRotSpeed;		//X軸回転速度
	float			m_fZoomSpeed;		//ズーム速度	
};
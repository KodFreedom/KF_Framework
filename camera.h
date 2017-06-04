//--------------------------------------------------------------------------------
//
//　Camera.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CCamera
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	enum CAMERA_DEFAULT
	{ 
		DEFAULT_FOV = 75,
		DEFAULT_FAR = 1000
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void LateUpdate(void);
	void Set(void);

	//Get関数
	CKFVec3		GetVecLook(void);
	CKFVec3		GetVecUp(void);
	CKFVec3		GetVecRight(void);
	CKFVec3		GetPosAt(void);
	CKFVec3		GetPosEye(void);
	D3DXMATRIX	GetMtxViewInverse(void);

	//Set関数
	void MoveCamera(const CKFVec3& vMovement);
	void LookAtHere(const CKFVec3& vPos);
	void SetCamera(const CKFVec3& vPosAt, const CKFVec3& vPosEye, const CKFVec3& vUp, const CKFVec3& vRight);
	void SetTarget(const CGameObject* pTarget);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void UpdateViewInverse(const D3DXMATRIX& mtxView);
	void Pitch(const float& fAngle);	//X軸(vVecRight)回転
	void Yaw(const float& fAngle);		//Y軸(vVecUp)回転
	void Roll(const float& fAngle);		//Z軸(vLook)回転
	void NormalizeCamera(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CKFVec3			m_vMovement;
	CKFVec3			m_vPosAt;			//注視点の位置
	CKFVec3			m_vPosEye;			//カメラの位置
	CKFVec3			m_vVecLook;			//カメラの前方向情報
	CKFVec3			m_vVecUp;			//カメラの上方向情報
	CKFVec3			m_vVecRight;		//カメラの右方向情報
	D3DXMATRIX		m_mtxViewInverse;	//Viewの逆行列
	float			m_fDistance;		//AtとEyeの距離
	float			m_fFovY;			//画角
	float			m_fFar;				//最大距離
	CGameObject*	m_pTarget;			//カメラが見てるターゲット
};
#endif
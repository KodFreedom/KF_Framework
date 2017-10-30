//--------------------------------------------------------------------------------
//	カメラクラス
//　Camera.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
#pragma once

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
#ifdef _DEBUG
	friend class CDebugManager;
#endif // _DEBUG


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

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Release(void)
	{
		Uninit();
		delete this;
	}

	void Set(void);

	//Get関数
	Vector3		GetVecLook(void);
	Vector3		GetVecUp(void);
	Vector3		GetVecRight(void);
	Vector3		GetPosAt(void);
	Vector3		GetPosEye(void);
	D3DXMATRIX	GetMtxViewInverse(void);

	//Set関数
	void MoveCamera(const Vector3& vMovement);
	void LookAtHere(const Vector3& vPos);
	void SetCamera(const Vector3& vPosAt, const Vector3& vPosEye, const Vector3& vUp, const Vector3& vRight);
	virtual void SetTarget(CGameObject* pTarget) {}

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void UpdateViewInverse(const D3DXMATRIX& mtxView);
	virtual void Pitch(const float& fAngle);	
	virtual void Yaw(const float& fAngle);		
	virtual void Roll(const float& fAngle);		
	virtual void NormalizeCamera(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Vector3			m_vMovement;
	Vector3			m_vPosAt;			//注視点の位置
	Vector3			m_vPosEye;			//カメラの位置
	Vector3			m_vVecLook;			//カメラの前方向情報
	Vector3			m_vVecUp;			//カメラの上方向情報
	Vector3			m_vVecRight;		//カメラの右方向情報
	D3DXMATRIX		m_mtxViewInverse;	//Viewの逆行列
	float			m_fDistance;		//AtとEyeの距離
	float			m_fFovY;			//画角
	float			m_fFar;				//最大距離
};

//--------------------------------------------------------------------------------
//  ヌルクラス
//--------------------------------------------------------------------------------
class CNullCamera : public CCamera
{
public:
	CNullCamera() : CCamera() {}
	~CNullCamera() {}

	void Init(void) override {}
	void Uninit(void) override {}
	void Update(void) override {}
	void LateUpdate(void) override {}
	void Release(void) override {}
};
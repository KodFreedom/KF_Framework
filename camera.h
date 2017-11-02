//--------------------------------------------------------------------------------
//	�J�����N���X
//�@Camera.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObject;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CCamera
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG


public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	enum CAMERA_DEFAULT
	{ 
		DEFAULT_FOV = 75,
		DEFAULT_FAR = 1000
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
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

	//Get�֐�
	Vector3		GetVecLook(void);
	Vector3		GetVecUp(void);
	Vector3		GetVecRight(void);
	Vector3		GetPosAt(void);
	Vector3		GetPosEye(void);
	D3DXMATRIX	GetMtxViewInverse(void);

	//Set�֐�
	void MoveCamera(const Vector3& vMovement);
	void LookAtHere(const Vector3& Position);
	void SetCamera(const Vector3& PositionAt, const Vector3& PositionEye, const Vector3& vUp, const Vector3& vRight);
	virtual void SetTarget(GameObject* pTarget) {}

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void UpdateViewInverse(const D3DXMATRIX& mtxView);
	virtual void Pitch(const float& fAngle);	
	virtual void Yaw(const float& fAngle);		
	virtual void Roll(const float& fAngle);		
	virtual void NormalizeCamera(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Vector3			m_vMovement;
	Vector3			PositionAt;			//�����_�̈ʒu
	Vector3			PositionEye;			//�J�����̈ʒu
	Vector3			m_vVecLook;			//�J�����̑O�������
	Vector3			m_vVecUp;			//�J�����̏�������
	Vector3			m_vVecRight;		//�J�����̉E�������
	D3DXMATRIX		m_mtxViewInverse;	//View�̋t�s��
	float			Distance;		//At��Eye�̋���
	float			m_fFovY;			//��p
	float			m_fFar;				//�ő勗��
};

//--------------------------------------------------------------------------------
//  �k���N���X
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
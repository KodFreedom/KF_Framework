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
class CGameObject;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CCamera
{
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
	CKFVec3		GetVecLook(void);
	CKFVec3		GetVecUp(void);
	CKFVec3		GetVecRight(void);
	CKFVec3		GetPosAt(void);
	CKFVec3		GetPosEye(void);
	D3DXMATRIX	GetMtxViewInverse(void);

	//Set�֐�
	void MoveCamera(const CKFVec3& vMovement);
	void LookAtHere(const CKFVec3& vPos);
	void SetCamera(const CKFVec3& vPosAt, const CKFVec3& vPosEye, const CKFVec3& vUp, const CKFVec3& vRight);
	virtual void SetTarget(CGameObject* pTarget) {}

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
	CKFVec3			m_vMovement;
	CKFVec3			m_vPosAt;			//�����_�̈ʒu
	CKFVec3			m_vPosEye;			//�J�����̈ʒu
	CKFVec3			m_vVecLook;			//�J�����̑O�������
	CKFVec3			m_vVecUp;			//�J�����̏�������
	CKFVec3			m_vVecRight;		//�J�����̉E�������
	D3DXMATRIX		m_mtxViewInverse;	//View�̋t�s��
	float			m_fDistance;		//At��Eye�̋���
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
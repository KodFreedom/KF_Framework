//--------------------------------------------------------------------------------
//
//�@Camera.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CCamera
{
public:
	enum CAMERA_DEFAULT
	{ 
		DEFAULT_FOV = 75,
		DEFAULT_FAR = 1000
	};

	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void LateUpdate(void);
	void Set(void);

	void MoveCamera(const CKFVec3 &vMovement);
	void LookAtHere(const CKFVec3 &vPos);
	void SetCamera(const CKFVec3 &vPosAt, const CKFVec3 &vPosEye, const CKFVec3 &vUp, const CKFVec3 &vRight);

	CKFVec3		GetVecLook(void);
	CKFVec3		GetVecUp(void);
	CKFVec3		GetVecRight(void);
	CKFVec3		GetPosAt(void);
	CKFVec3		GetPosEye(void);
	D3DXMATRIX	GetMtxViewInverse(void);

private:
	void UpdateViewInverse(const D3DXMATRIX &mtxView);
	void Pitch(const float &fAngle);	//X��(vVecRight)��]
	void Yaw(const float &fAngle);		//Y��(vVecUp)��]
	void Roll(const float &fAngle);		//Z��(vLook)��]
	void NormalizeCamera(void);

	CKFVec3		m_vMovement;
	CKFVec3		m_vPosAt;			//�����_�̈ʒu
	CKFVec3		m_vPosEye;			//�J�����̈ʒu
	CKFVec3		m_vVecLook;			//�J�����̑O�������
	CKFVec3		m_vVecUp;			//�J�����̏�������
	CKFVec3		m_vVecRight;		//�J�����̉E�������
	D3DXMATRIX	m_mtxViewInverse;	//View�̋t�s��
	float		m_fDistance;		//At��Eye�̋���
	float		m_fFovY;			//��p
	float		m_fFar;				//�ő勗��
};
#endif
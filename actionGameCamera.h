//--------------------------------------------------------------------------------
//	�A�N�V�����Q�[���p�J�����N���X
//�@actionGameCamera.h
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "camera.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CActionGameCamera : public CCamera
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CActionGameCamera();
	~CActionGameCamera() {}
	
	void Init(void) override;
	void Update(void) override;
	void LateUpdate(void) override;

	//Set�֐�
	void SetTarget(CGameObject* pTarget) override { m_pTarget = pTarget; }

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const float		sc_fRotSpeed;		//�J������]���x
	static const float		sc_fStartRotMin;	//�J������]�J�n�̃X�e�B�b�N�ŏ��l
	static const float		sc_fRotLerpTime;	//�J������]���x�x�������W��
	static const float		sc_fZoomSpeed;		//�Y�[�����x
	static const float		sc_fDistanceMin;	//�����_�ƒ��ړ_�̍ŏ�����
	static const float		sc_fDistanceDef;	//�����_�ƒ��ړ_�̃f�t�H���g�l
	static const float		sc_fDistanceMax;	//�����_�ƒ��ړ_�̍ő勗��
	static const float		sc_fZoomLerpTime;	//�Y�[���x�������W��
	static const float		sc_fMoveLerpTime;	//�ړ��x�������W��
	static const float		sc_fReRotLerpTime;	//�p�x�߂��x�������W��	
	static const float		sc_fRotXMax;		//X����]�̍ő�p�x
	static const float		sc_fRotXDefault;	//X����]�̃f�t�H���g�l
	static const float		sc_fCollisionRadius;//�����蔻��p�̔��a
	static const float		sc_fPosAtHeight;	//�����_�̍���

	//--------------------------------------------------------------------------------
	//  �֐���`
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
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CKFVec3			m_vPosAtNext;		//�����_�̈ʒu
	CKFVec3			m_vPosEyeNext;		//�J�����̈ʒu
	CKFVec3			m_vVecLookNext;		//�J�����̑O�������
	CKFVec3			m_vVecUpNext;		//�J�����̏�������
	CKFVec3			m_vVecRightNext;	//�J�����̉E�������
	float			m_fDistanceNext;	//At��Eye�̋���

	CGameObject*	m_pTarget;			//�J���������Ă�^�[�Q�b�g
	CKFVec3			m_vDefaultLook;		//�f�t�H���g�O����
	CKFVec3			m_vRotSpeed;		//X����]���x
	float			m_fZoomSpeed;		//�Y�[�����x	
};
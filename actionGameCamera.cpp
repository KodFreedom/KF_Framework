//--------------------------------------------------------------------------------
//	�A�N�V�����Q�[���p�J�����N���X
//�@actionGameCamera.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "actionGameCamera.h"
#include "manager.h"
#include "rendererDX.h"
#include "inputManager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float	CActionGameCamera::sc_fRotSpeed = 0.05f;
const float	CActionGameCamera::sc_fStartRotMin = 0.2f;
const float	CActionGameCamera::sc_fRotLerpTime = 0.1f;
const float	CActionGameCamera::sc_fZoomSpeed = 0.25f;
const float	CActionGameCamera::sc_fDistanceMin = 3.0f;
const float	CActionGameCamera::sc_fDistanceDef = 6.0f;
const float	CActionGameCamera::sc_fDistanceMax = 10.0f;
const float	CActionGameCamera::sc_fZoomLerpTime = 0.2f;
const float	CActionGameCamera::sc_fMoveLerpTime = 0.075f;
const float	CActionGameCamera::sc_fReRotLerpTime = 0.025f;
const float	CActionGameCamera::sc_fRotXMax = 60.0f / 180.0f * KF_PI;
const float	CActionGameCamera::sc_fRotXDefault = 15.0f / 180.0f * KF_PI;
const float	CActionGameCamera::sc_fCollisionRadius = 0.1f;
const float CActionGameCamera::sc_fPosAtHeight = 1.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CActionGameCamera::CActionGameCamera() : CCamera()
	, m_vRotSpeed(CKFVec3(0.0f))
	, m_fZoomSpeed(0.0f)
	, m_vPosAtNext(CKFVec3(0.0f))
	, m_vPosEyeNext(CKFVec3(0.0f))
	, m_vVecLookNext(CKFVec3(0.0f))
	, m_vVecUpNext(CKFVec3(0.0f))
	, m_vVecRightNext(CKFVec3(0.0f))
	, m_fDistanceNext(0.0f)
	, m_vDefaultLook(CKFVec3(0.0f))
	, m_pTarget(nullptr)
{
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CActionGameCamera::Init(void)
{
	CCamera::Init();

	//�_�u���o�b�t�@�����l�ݒ�
	m_vPosAtNext = m_vPosAt;
	m_vPosEyeNext = m_vPosEye;
	m_vVecLookNext = m_vVecLook;
	m_vVecRightNext = m_vVecRight;
	m_vVecUpNext = m_vVecUp;
	m_fDistanceNext = m_fDistance;

	//�f�t�H���g�p�x�ݒ�
	m_vDefaultLook = CKFVec3(0.0f, -tanf(sc_fRotXDefault), 1.0f);
	CKFMath::VecNormalize(m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CActionGameCamera::Update(void)
{
	SwitchParam();

	CInputManager *pInput = CMain::GetManager()->GetInputManager();
	CKFVec3 vRot = CKFVec3(0.0f);
	float fZoomSpeed = 0.0f;

	float fRAxisX = pInput->GetRotHorizontal();
	float fRAxisY = pInput->GetRotVertical();
	float fZoom = pInput->GetZoom();

	//���ړ_��]
	if (fabsf(fRAxisX) > sc_fStartRotMin)
	{//Y����]
		vRot.m_fY = sc_fRotSpeed * fRAxisX;
	}
	if (fabsf(fRAxisY) > sc_fStartRotMin)
	{//X����]
		vRot.m_fX = sc_fRotSpeed * fRAxisY;
	}

	//�g��k��
	if (fabsf(fZoom) > sc_fStartRotMin)
	{
		fZoomSpeed = sc_fZoomSpeed * fZoom;
	}

	//��]���x�Z�o
	m_vRotSpeed = CKFMath::LerpVec3(m_vRotSpeed, vRot, sc_fRotLerpTime);
	
	//��]�𐧌�����
	LimitRot();

	//��]
	Yaw(m_vRotSpeed.m_fY);
	Pitch(m_vRotSpeed.m_fX);

	//Target�ړ�������J�����̊p�x���f�t�H���g�̊p�x�ɂ��񂾂�C�����ɍs��
	//if (m_pTarget && m_pTarget->GetTransformComponent()->GetPos() != m_pTarget->GetTransformComponent()->GetPosNext())
	//{
	//	m_vVecLookNext = CKFMath::LerpNormal(m_vVecLookNext, m_vDefaultLook, sc_fReRotLerpTime);
	//}

	//���E�Ƃ̏Փ˔���
	//CheckCollision();



	//�Y�[��
	/*m_fZoomSpeed = CKFMath::LerpFloat(m_fZoomSpeed, fZoomSpeed, sc_fZoomLerpTime);
	m_fDistance += m_fZoomSpeed;
	m_fDistance = m_fDistance < sc_fDistanceMin ? sc_fDistanceMin : m_fDistance > sc_fDistanceMax ? sc_fDistanceMax : m_fDistance;*/

	//if (!CMain::GetManager()->GetColliderManager()->CheckActionCameraCollision(this))
	//{
	//	m_fDistance = CKFMath::LerpFloat(m_fDistance, sc_fDistanceDef, sc_fZoomLerpTime);
	//}

	
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CActionGameCamera::LateUpdate(void)
{
	//�J�����ړ�
	if (m_pTarget)
	{
		m_vPosAtNext = CKFMath::LerpVec3(m_vPosAtNext, m_pTarget->GetTransformComponent()->GetPos() + CKFVec3(0.0f, 1.0f, 0.0f) * sc_fPosAtHeight, sc_fMoveLerpTime);
	}

	//�J�����m�[�}���C�Y
	NormalizeCamera();
	m_vPosEyeNext = m_vPosAtNext - m_vVecLookNext * m_fDistanceNext;
}

//--------------------------------------------------------------------------------
//  Y����]
//--------------------------------------------------------------------------------
void CActionGameCamera::Yaw(const float& fAngle)
{
	CKFMtx44 mtxYaw;
	CKFVec3 vAxisY = CKFVec3(0.0f, 1.0f, 0.0f);
	CKFMath::MtxRotAxis(mtxYaw, vAxisY, fAngle);
	m_vVecRightNext = CKFMath::Vec3TransformNormal(m_vVecRightNext, mtxYaw);
	m_vVecLookNext = CKFMath::Vec3TransformNormal(m_vVecLookNext, mtxYaw);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  X����]
//--------------------------------------------------------------------------------
void CActionGameCamera::Pitch(const float& fAngle)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vVecRightNext, fAngle);
	m_vVecUpNext = CKFMath::Vec3TransformNormal(m_vVecUpNext, mtxPitch);
	m_vVecLookNext = CKFMath::Vec3TransformNormal(m_vVecLookNext, mtxPitch);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  �J�����̃m�[�}���C�Y
//--------------------------------------------------------------------------------
void CActionGameCamera::NormalizeCamera(void)
{
	CKFMath::VecNormalize(m_vVecLookNext);
	m_vVecUpNext = m_vVecLookNext * m_vVecRightNext;
	CKFMath::VecNormalize(m_vVecUpNext);
	m_vVecRightNext = m_vVecUpNext * m_vVecLookNext;
	CKFMath::VecNormalize(m_vVecRightNext);
	ReCalcDefault();
}

//--------------------------------------------------------------------------------
//  �J�����̉�]����
//--------------------------------------------------------------------------------
void CActionGameCamera::ReCalcDefault(void)
{
	m_vDefaultLook = m_vVecRightNext * (m_vDefaultLook * m_vVecRightNext);
	CKFMath::VecNormalize(m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  X����]�p�x�Z�o
//--------------------------------------------------------------------------------
float CActionGameCamera::CalcRotX(void)
{
	CKFVec2 vX = CKFVec2(m_vVecLook.m_fX, m_vVecLook.m_fZ);
	float fRot = atan2f(-m_vVecLook.m_fY, CKFMath::VecMagnitude(vX));
	return fRot;
}

//--------------------------------------------------------------------------------
//  �J������]�𐧌�����
//--------------------------------------------------------------------------------
void CActionGameCamera::LimitRot(void)
{
	//X����]����
	float fRotXNow = CalcRotX();
	if (m_vRotSpeed.m_fX + fRotXNow >= sc_fRotXMax)
	{
		m_vRotSpeed.m_fX = sc_fRotXMax - fRotXNow;
	}
	else if (m_vRotSpeed.m_fX + fRotXNow <= -sc_fRotXMax)
	{
		m_vRotSpeed.m_fX = -sc_fRotXMax - fRotXNow;
	}
}

//--------------------------------------------------------------------------------
//  �ϐ�����
//--------------------------------------------------------------------------------
void CActionGameCamera::SwitchParam(void)
{
	m_vPosAt = m_vPosAtNext;
	m_vPosEye = m_vPosEyeNext;
	m_vVecLook = m_vVecLookNext;
	m_vVecUp = m_vVecUpNext;
	m_vVecRight = m_vVecRightNext;
}

//--------------------------------------------------------------------------------
//  �}�b�v�Ƃ̃R���W�����`�F�b�N
//--------------------------------------------------------------------------------
void CActionGameCamera::CheckCollision(void)
{
	//CColliderManager::HIT_INFO hitInfo;
	//m_vPosEyeNext = m_vPosAtNext - m_vVecLookNext * m_fDistanceNext;

	//if (CMain::GetManager()->GetColliderManager()->SphereCast(m_vPosEyeNext, sc_fCollisionRadius, hitInfo))
	//{//�Փ˂�����
	//	//�V�����J�����̎Z�o-
	//	CKFVec3 vNewPosEye = hitInfo.vPos + hitInfo.vNormal * sc_fCollisionRadius;
	//	
	//	//�V�����O�����̎Z�o
	//	m_vVecLookNext = m_vPosAtNext - vNewPosEye;

	//	//�V���������̎Z�o
	//	m_fDistanceNext = CKFMath::VecMagnitude(m_vVecLookNext);

	//	//if (m_vVecLookNext.m_fY != 0.0f)
	//	//{
	//	//	fNewDis = fabsf((vNewPos.m_fY - m_vPosAtNext.m_fY) / m_vVecLookNext.m_fY);
	//	//}
	//	//m_fDistanceNext = CKFMath::LerpFloat(m_fDistanceNext, fNewDis, sc_fZoomLerpTime);
	//}
	//else
	//{
	//	m_fDistanceNext = CKFMath::LerpFloat(m_fDistanceNext, sc_fDistanceDef, sc_fZoomLerpTime);
	//}
}
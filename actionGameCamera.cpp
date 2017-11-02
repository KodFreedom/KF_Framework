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
	, m_vRotSpeed(Vector3(0.0f))
	, ZoomSpeed(0.0f)
	, PositionAtNext(Vector3(0.0f))
	, PositionEyeNext(Vector3(0.0f))
	, m_vVecLookNext(Vector3(0.0f))
	, m_vVecUpNext(Vector3(0.0f))
	, m_vVecRightNext(Vector3(0.0f))
	, DistanceNext(0.0f)
	, m_vDefaultLook(Vector3(0.0f))
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
	PositionAtNext = PositionAt;
	PositionEyeNext = PositionEye;
	m_vVecLookNext = m_vVecLook;
	m_vVecRightNext = m_vVecRight;
	m_vVecUpNext = m_vVecUp;
	DistanceNext = Distance;

	//�f�t�H���g�p�x�ݒ�
	m_vDefaultLook = Vector3(0.0f, -tanf(sc_fRotXDefault), 1.0f);
	CKFMath::VecNormalize(m_vDefaultLook);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CActionGameCamera::Update(void)
{
	SwitchParam();

	Input *pInput = Main::GetManager()->GetInputManager();
	Vector3 vRot = Vector3(0.0f);
	float fZoomSpeed = 0.0f;

	float fRAxisX = pInput->GetRotHorizontal();
	float fRAxisY = pInput->GetRotVertical();
	float fZoom = pInput->GetZoom();

	//���ړ_��]
	if (fabsf(fRAxisX) > sc_fStartRotMin)
	{//Y����]
		vRot.Y = sc_fRotSpeed * fRAxisX;
	}
	if (fabsf(fRAxisY) > sc_fStartRotMin)
	{//X����]
		vRot.Y = sc_fRotSpeed * fRAxisY;
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
	Yaw(m_vRotSpeed.Y);
	Pitch(m_vRotSpeed.Y);

	//Target�ړ�������J�����̊p�x���f�t�H���g�̊p�x�ɂ��񂾂�C�����ɍs��
	//if (m_pTarget && m_pTarget->GetTransformComponent()->GetPos() != m_pTarget->GetTransformComponent()->GetPosNext())
	//{
	//	m_vVecLookNext = CKFMath::LerpNormal(m_vVecLookNext, m_vDefaultLook, sc_fReRotLerpTime);
	//}

	//���E�Ƃ̏Փ˔���
	//CheckCollision();



	//�Y�[��
	/*ZoomSpeed = CKFMath::LerpFloat(ZoomSpeed, fZoomSpeed, sc_fZoomLerpTime);
	Distance += ZoomSpeed;
	Distance = Distance < sc_fDistanceMin ? sc_fDistanceMin : Distance > sc_fDistanceMax ? sc_fDistanceMax : Distance;*/

	//if (!Main::GetManager()->GetColliderManager()->CheckActionCameraCollision(this))
	//{
	//	Distance = CKFMath::LerpFloat(Distance, sc_fDistanceDef, sc_fZoomLerpTime);
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
		PositionAtNext = CKFMath::LerpVec3(PositionAtNext, m_pTarget->GetTransformComponent()->GetPos() + Vector3(0.0f, 1.0f, 0.0f) * sc_fPosAtHeight, sc_fMoveLerpTime);
	}

	//�J�����m�[�}���C�Y
	NormalizeCamera();
	PositionEyeNext = PositionAtNext - m_vVecLookNext * DistanceNext;
}

//--------------------------------------------------------------------------------
//  Y����]
//--------------------------------------------------------------------------------
void CActionGameCamera::Yaw(const float& fAngle)
{
	Matrix44 mtxYaw;
	Vector3 vAxisY = Vector3(0.0f, 1.0f, 0.0f);
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
	Matrix44 mtxPitch;
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
	Vector2 vX = Vector2(m_vVecLook.Y, m_vVecLook.Z);
	float fRot = atan2f(-m_vVecLook.Y, CKFMath::VecMagnitude(vX));
	return fRot;
}

//--------------------------------------------------------------------------------
//  �J������]�𐧌�����
//--------------------------------------------------------------------------------
void CActionGameCamera::LimitRot(void)
{
	//X����]����
	float fRotXNow = CalcRotX();
	if (m_vRotSpeed.Y + fRotXNow >= sc_fRotXMax)
	{
		m_vRotSpeed.Y = sc_fRotXMax - fRotXNow;
	}
	else if (m_vRotSpeed.Y + fRotXNow <= -sc_fRotXMax)
	{
		m_vRotSpeed.Y = -sc_fRotXMax - fRotXNow;
	}
}

//--------------------------------------------------------------------------------
//  �ϐ�����
//--------------------------------------------------------------------------------
void CActionGameCamera::SwitchParam(void)
{
	PositionAt = PositionAtNext;
	PositionEye = PositionEyeNext;
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
	//PositionEyeNext = PositionAtNext - m_vVecLookNext * DistanceNext;

	//if (Main::GetManager()->GetColliderManager()->SphereCast(PositionEyeNext, sc_fCollisionRadius, hitInfo))
	//{//�Փ˂�����
	//	//�V�����J�����̎Z�o-
	//	Vector3 vNewPosEye = hitInfo.Position + hitInfo.Normal * sc_fCollisionRadius;
	//	
	//	//�V�����O�����̎Z�o
	//	m_vVecLookNext = PositionAtNext - vNewPosEye;

	//	//�V���������̎Z�o
	//	DistanceNext = CKFMath::VecMagnitude(m_vVecLookNext);

	//	//if (m_vVecLookNext.Y != 0.0f)
	//	//{
	//	//	fNewDis = fabsf((vNewPos.Y - PositionAtNext.Y) / m_vVecLookNext.Y);
	//	//}
	//	//DistanceNext = CKFMath::LerpFloat(DistanceNext, fNewDis, sc_fZoomLerpTime);
	//}
	//else
	//{
	//	DistanceNext = CKFMath::LerpFloat(DistanceNext, sc_fDistanceDef, sc_fZoomLerpTime);
	//}
}
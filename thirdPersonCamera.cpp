//--------------------------------------------------------------------------------
//	�O�l�̃J�����N���X
//�@ThirdPersionCamera.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "thirdPersonCamera.h"
#include "manager.h"
#include "input.h"
#include "gameObject.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float	ThirdPersionCamera::rotationSpeed = 0.05f;
const float	ThirdPersionCamera::startRotationMin = 0.2f;
const float	ThirdPersionCamera::rotationLerpTime = 0.1f;
const float	ThirdPersionCamera::distanceDefault = 6.0f;
const float	ThirdPersionCamera::distanceMin = 3.0f;
const float	ThirdPersionCamera::distanceMax = 10.0f;
const float	ThirdPersionCamera::moveLerpTime = 0.075f;
const float	ThirdPersionCamera::pitchDefault = 15.0f / 180.0f * Pi;
const float	ThirdPersionCamera::pitchMin = -10.0f / 180.0f * Pi;
const float	ThirdPersionCamera::pitchMax = 60.0f / 180.0f * Pi;
const float	ThirdPersionCamera::collisionRadius = 0.1f;
const float ThirdPersionCamera::offsetY = 1.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ThirdPersionCamera::ThirdPersionCamera() : Camera()
	, followTarget(nullptr)
	, pitchSpeed(0.0f)
	, yawSpeed(0.0f)
{
	CameraManager::Instance()->RegisterAsMain(this);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Init(void)
{
	Camera::Init();
	SetOffsetY(offsetY);
	SetDistance(distanceDefault);
	SetPitch(pitchDefault);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Update(void)
{
	auto input = Input::Instance();
	float rotationAxisX = input->RotHorizontal();
	float rotationAxisY = input->RotVertical();

	float pitchAmount = 0.0f;
	float yawAmount = 0.0f;

	if (fabsf(rotationAxisX) > startRotationMin)
	{//Y����]
		yawAmount = rotationSpeed * rotationAxisX;
	}
	if (fabsf(rotationAxisY) > startRotationMin)
	{//X����]
		pitchAmount = rotationSpeed * rotationAxisY;
	}

	pitchSpeed = Math::Lerp(pitchSpeed, pitchAmount, rotationLerpTime);
	yawSpeed = Math::Lerp(yawSpeed, yawAmount, rotationLerpTime);
	pitch(pitchSpeed);
	yaw(yawSpeed);
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void ThirdPersionCamera::LateUpdate(void)
{
	if (followTarget)
	{
		rig.Position = Math::Lerp(rig.Position, followTarget->GetTransform()->GetPos(), moveLerpTime);
	}

	Camera::LateUpdate();
}

//--------------------------------------------------------------------------------
//  X����]
//--------------------------------------------------------------------------------
void ThirdPersionCamera::pitch(const float& radian)
{
	Camera::pitch(radian);
	pivot.Rotation.X = Math::Clamp(pivot.Rotation.X, pitchMin, pitchMax);
}
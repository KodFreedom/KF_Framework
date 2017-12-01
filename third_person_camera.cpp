//--------------------------------------------------------------------------------
//�@third_person_camera.h
//	�O�l�̃J�����N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "third_person_camera.h"
#include "main_system.h"
#include "input.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ThirdPersionCamera::ThirdPersionCamera() : Camera()
	, follow_target_(nullptr)
	, pitch_speed_(0.0f)
	, yaw_speed_(0.0f)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Init(void)
{
	Camera::Init();
	SetOffsetY(kOffsetY);
	SetDistance(kDistanceDefault);
	SetPitch(kPitchDefault);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Update(void)
{
	auto input = MainSystem::Instance()->GetInput();
	float rotation_x = input->RotationHorizontal();
	float rotation_y = input->RotationVertical();

	float pitch_amount = 0.0f;
	float yaw_amount = 0.0f;

	if (fabsf(rotation_x) > kStartRotationMin)
	{//Y����]
		yaw_amount = kRotationSpeed * rotation_x;
	}
	if (fabsf(rotation_y) > kStartRotationMin)
	{//X����]
		pitch_amount = kRotationSpeed * rotation_y;
	}

	pitch_speed_ = math::Lerp(pitch_speed_, pitch_amount, kRotationLerpTime);
	yaw_speed_ = math::Lerp(yaw_speed_, yaw_amount, kRotationLerpTime);
	Pitch(pitch_speed_);
	Yaw(yaw_speed_);
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void ThirdPersionCamera::LateUpdate(void)
{
	if (follow_target_)
	{
		rig_.position = math::Lerp(rig_.position, follow_target_->GetTransform()->GetPosition(), kMoveLerpTime);
	}
	Camera::LateUpdate();
}

//--------------------------------------------------------------------------------
//  X����]
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Pitch(const float& radian)
{
	Camera::Pitch(radian);
	pivot_.rotation.x_ = math::Clamp(pivot_.rotation.x_, kPitchMin, kPitchMax);
}
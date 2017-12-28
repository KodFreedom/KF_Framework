//--------------------------------------------------------------------------------
//�@camera.cpp
//	�J�������N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "camera.h"
#include "main_system.h"
#include "camera_manager.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Camera::Camera()
	: local_eye_position_(Vector3::kZero)
	, world_eye_position_(Vector3::kZero)
	, world_at_position_(Vector3::kZero)
	, world_right_(Vector3::kZero)
	, world_up_(Vector3::kZero)
	, world_forward_(Vector3::kZero)
	, view_(Matrix44::kIdentity)
	, view_transpose_(Matrix44::kIdentity)
	, projection_(Matrix44::kIdentity)
	, distance_(0.0f)
	, fov_(75.0f / 180.0f * kPi)
	, near_(0.1f)
	, far_(kDefaultfar_)
{
	rig_.position = Vector3::kZero;
	rig_.rotation = Vector3::kZero;
	pivot_.position = Vector3::kZero;
	pivot_.rotation = Vector3::kZero;
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void Camera::Init(void)
{
	MainSystem::Instance()->GetCameraManager()->RegisterAsMain(this);
}

//--------------------------------------------------------------------------------
//  �Z�b�g����(�`�撼�O)
//--------------------------------------------------------------------------------
void Camera::Set(void)
{
	// View�s��
	auto& negative_eye = world_eye_position_ * -1.0f;
	view_transpose_ = Matrix44(
		world_right_.x_, world_right_.y_, world_right_.z_, world_right_.Dot(negative_eye),
		world_up_.x_, world_up_.y_, world_up_.z_, world_up_.Dot(negative_eye),
		world_forward_.x_, world_forward_.y_, world_forward_.z_, world_forward_.Dot(negative_eye),
		0.0f, 0.0f, 0.0f, 1.0f);
	view_ = view_transpose_.Transpose();

	// Projection�s��
	projection_ = Matrix44::PerspectiveLeftHand(fov_, (float)SCREEN_WIDTH / SCREEN_HEIGHT, near_, far_);
}

//--------------------------------------------------------------------------------
//  �J�����͈͓̔��ɓ����Ă邩�ǂ���
//--------------------------------------------------------------------------------
bool Camera::IsInRange(const Vector3& position, const float& radius)
{ // Todo : �J�����O�㔻��A���͈̂̔͂�����A����p�̔���...
	return Vector3::SquareDistanceBetween(rig_.position, position) <= radius * radius;
}

//--------------------------------------------------------------------------------
//
//  protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �p�����[�^�[�̎Z�o
//--------------------------------------------------------------------------------
void Camera::UpdateParameter(void)
{
	auto& world_rig = Matrix44::Transform(rig_.rotation, rig_.position);
	auto& world_pivot = Matrix44::Transform(pivot_.rotation, pivot_.position);
	world_pivot *= world_rig;

	world_forward_ = Vector3::TransformNormal(Vector3::kForward, world_pivot).Normalized();
	world_right_ = (Vector3::kUp * world_forward_).Normalized();
	world_up_ = (world_forward_ * world_right_).Normalized();
	
	world_eye_position_ = Vector3::TransformCoord(local_eye_position_, world_pivot);
	world_at_position_ = world_eye_position_ + world_forward_ * distance_;
}
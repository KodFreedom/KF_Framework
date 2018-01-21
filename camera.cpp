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
    ZeroMemory(planes_, sizeof(Vector4) * kCameraPlaneMax);
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
    UpdateParameter();
    ConstructFrustum();
}

//--------------------------------------------------------------------------------
//  �J�����͈͓̔��ɓ����Ă邩�ǂ���(������J�����O)
//--------------------------------------------------------------------------------
bool Camera::FrustumCulling(const Vector3& position, const float& radius)
{ 
    for (int count = 0; count < kCameraPlaneMax; ++count)
    {
        if (planes_[count].PlaneDotCoord(position) < -radius)
        {
            return false;
        }
    }
    return true;
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

    // View�s��
    auto& negative_eye = world_eye_position_ * -1.0f;
    view_transpose_ = Matrix44(
        world_right_.x_, world_right_.y_, world_right_.z_, world_right_.Dot(negative_eye),
        world_up_.x_, world_up_.y_, world_up_.z_, world_up_.Dot(negative_eye),
        world_forward_.x_, world_forward_.y_, world_forward_.z_, world_forward_.Dot(negative_eye),
        0.0f, 0.0f, 0.0f, 1.0f);
    view_ = view_transpose_.Transpose();
    view_transpose_.m03_ = 0.0f;
    view_transpose_.m13_ = 0.0f;
    view_transpose_.m23_ = 0.0f;
    view_transpose_.m33_ = 1.0f;

    // Projection�s��
    projection_ = Matrix44::PerspectiveLeftHand(fov_, (float)SCREEN_WIDTH / SCREEN_HEIGHT, near_, far_);
}

//--------------------------------------------------------------------------------
//  ������̎Z�o
//--------------------------------------------------------------------------------
void Camera::ConstructFrustum(void)
{
    Matrix44 fixed_projection = projection_;

    // �������near�̎Z�o
    float min_z = -fixed_projection.m33_ / fixed_projection.m23_;
    float range = far_ / (far_ - min_z);
    fixed_projection.m23_ = range;
    fixed_projection.m33_ = -range * min_z;

    // ������s��̎Z�o
    Matrix44& frustum_matrix = view_ * fixed_projection;

    // Near����
    planes_[kCameraNear].x_ = frustum_matrix.m03_ + frustum_matrix.m02_;
    planes_[kCameraNear].y_ = frustum_matrix.m13_ + frustum_matrix.m12_;
    planes_[kCameraNear].z_ = frustum_matrix.m23_ + frustum_matrix.m22_;
    planes_[kCameraNear].w_ = frustum_matrix.m33_ + frustum_matrix.m32_;
    planes_[kCameraNear].PlaneNormalize();

    // Far����
    planes_[kCameraFar].x_ = frustum_matrix.m03_ - frustum_matrix.m02_;
    planes_[kCameraFar].y_ = frustum_matrix.m13_ - frustum_matrix.m12_;
    planes_[kCameraFar].z_ = frustum_matrix.m23_ - frustum_matrix.m22_;
    planes_[kCameraFar].w_ = frustum_matrix.m33_ - frustum_matrix.m32_;
    planes_[kCameraFar].PlaneNormalize();

    // Left����
    planes_[kCameraLeft].x_ = frustum_matrix.m03_ + frustum_matrix.m00_;
    planes_[kCameraLeft].y_ = frustum_matrix.m13_ + frustum_matrix.m10_;
    planes_[kCameraLeft].z_ = frustum_matrix.m23_ + frustum_matrix.m20_;
    planes_[kCameraLeft].w_ = frustum_matrix.m33_ + frustum_matrix.m30_;
    planes_[kCameraLeft].PlaneNormalize();

    // Right����
    planes_[kCameraRight].x_ = frustum_matrix.m03_ - frustum_matrix.m00_;
    planes_[kCameraRight].y_ = frustum_matrix.m13_ - frustum_matrix.m10_;
    planes_[kCameraRight].z_ = frustum_matrix.m23_ - frustum_matrix.m20_;
    planes_[kCameraRight].w_ = frustum_matrix.m33_ - frustum_matrix.m30_;
    planes_[kCameraRight].PlaneNormalize();

    // Top����
    planes_[kCameraTop].x_ = frustum_matrix.m03_ - frustum_matrix.m01_;
    planes_[kCameraTop].y_ = frustum_matrix.m13_ - frustum_matrix.m11_;
    planes_[kCameraTop].z_ = frustum_matrix.m23_ - frustum_matrix.m21_;
    planes_[kCameraTop].w_ = frustum_matrix.m33_ - frustum_matrix.m31_;
    planes_[kCameraTop].PlaneNormalize();

    // Bottom����
    planes_[kCameraBottom].x_ = frustum_matrix.m03_ + frustum_matrix.m01_;
    planes_[kCameraBottom].y_ = frustum_matrix.m13_ + frustum_matrix.m11_;
    planes_[kCameraBottom].z_ = frustum_matrix.m23_ + frustum_matrix.m21_;
    planes_[kCameraBottom].w_ = frustum_matrix.m33_ + frustum_matrix.m31_;
    planes_[kCameraBottom].PlaneNormalize();
}
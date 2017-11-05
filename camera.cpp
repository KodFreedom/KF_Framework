//--------------------------------------------------------------------------------
//	�J�����N���X
//�@Camera.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "camera.h"
#include "renderer.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//  Camera�N���X
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void Camera::LateUpdate(void)
{
	//NormalizeCamera();
	//PositionEye = PositionAt - forward * Distance;
	updateParamater();
}

//--------------------------------------------------------------------------------
//  �Z�b�g����(�`�撼�O)
//--------------------------------------------------------------------------------
void Camera::Set(void)
{
	// View�s��
	auto& negativeEye = worldPositionEye * -1.0f;
	viewTranspose = Matrix44(
		worldRight.X, worldRight.Y, worldRight.Z, worldRight.Dot(negativeEye),
		worldUp.X, worldUp.Y, worldUp.Z, worldUp.Dot(negativeEye),
		worldForward.X, worldForward.Y, worldForward.Z, worldForward.Dot(negativeEye),
		0.0f, 0.0f, 0.0f, 1.0f);
	view = viewTranspose.Transpose();

	// Projection�s��
	auto& projection = Matrix44::ProjectionLeftHand(fovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, nearZ, farZ);

	Renderer::Instance()->SetPorjectionCamera(view, projection);
}

//--------------------------------------------------------------------------------
//  �J�����̕`��͈͓��ɓ����Ă邩�ǂ���
//--------------------------------------------------------------------------------
bool Camera::IsInRange(const Vector3& position)
{ // Todo : �J�����O�㔻��A���͈̂̔͂�����A����p�̔���...
	return Vector3::SquareDistanceBetween(rig.Position, position) > farZ * farZ;
}

//--------------------------------------------------------------------------------
//  �ړ�����
//--------------------------------------------------------------------------------
void Camera::Move(const Vector3& movement)
{
	rig.Position += movement;
}

//--------------------------------------------------------------------------------
//
//  protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Camera::Camera()
	: localPositionEye(Vector3::Zero)
	, worldPositionEye(Vector3::Zero)
	, worldPositionAt(Vector3::Zero)
	, worldRight(Vector3::Zero)
	, worldUp(Vector3::Zero)
	, worldForward(Vector3::Zero)
	, view(Matrix44::Identity)
	, viewTranspose(Matrix44::Identity)
	, distance(0.0f)
	, fovY(75.0f / 180.0f * Pi)
	, nearZ(0.1f)
	, farZ(1000.0f)
{
	rig.Position = Vector3::Zero;
	rig.Rotation = Vector3::Zero;
	pivot.Position = Vector3::Zero;
	pivot.Rotation = Vector3::Zero;
}

//--------------------------------------------------------------------------------
//  �J�����O���Ə�ƉE�����90�x�ɂȂ��Ă���m�ۂ���
//--------------------------------------------------------------------------------
//void Camera::normalize(void)
//{
//	forward.Normalize();
//	up = (forward * right).Normalized();
//	right = (up * forward).Normalized();
//}

//--------------------------------------------------------------------------------
//  X����]
//--------------------------------------------------------------------------------
void Camera::pitch(const float& radian)
{
	pivot.Rotation.X += radian;
}

//--------------------------------------------------------------------------------
//  Y����]
//--------------------------------------------------------------------------------
void Camera::yaw(const float& radian)
{
	rig.Rotation.Y += radian;
}

//--------------------------------------------------------------------------------
//  �p�����[�^�[�̎Z�o
//--------------------------------------------------------------------------------
void Camera::updateParamater(void)
{
	auto& worldRig = Matrix44::Transform(rig.Rotation, rig.Position);
	auto& worldPivot = Matrix44::Transform(pivot.Rotation, pivot.Position);
	worldPivot *= worldRig;
	worldRight = Vector3::TransformNormal(Vector3::Right, worldPivot);
	worldUp = Vector3::TransformNormal(Vector3::Up, worldPivot);
	worldForward = Vector3::TransformNormal(Vector3::Forward, worldPivot);
	worldPositionEye = Vector3::TransformCoord(localPositionEye, worldPivot);
	worldPositionAt = worldPositionEye + worldForward * distance;
}

//--------------------------------------------------------------------------------
//
//  NormalCamera�N���X
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
NormalCamera::NormalCamera() : Camera()
{
	CameraManager::Instance()->RegisterAsMain(this);
}
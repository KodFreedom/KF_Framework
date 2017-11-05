//--------------------------------------------------------------------------------
//	カメラクラス
//　Camera.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "camera.h"
#include "renderer.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//  Cameraクラス
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void Camera::LateUpdate(void)
{
	//NormalizeCamera();
	//PositionEye = PositionAt - forward * Distance;
	updateParamater();
}

//--------------------------------------------------------------------------------
//  セット処理(描画直前)
//--------------------------------------------------------------------------------
void Camera::Set(void)
{
	// View行列
	auto& negativeEye = worldPositionEye * -1.0f;
	viewTranspose = Matrix44(
		worldRight.X, worldRight.Y, worldRight.Z, worldRight.Dot(negativeEye),
		worldUp.X, worldUp.Y, worldUp.Z, worldUp.Dot(negativeEye),
		worldForward.X, worldForward.Y, worldForward.Z, worldForward.Dot(negativeEye),
		0.0f, 0.0f, 0.0f, 1.0f);
	view = viewTranspose.Transpose();

	// Projection行列
	auto& projection = Matrix44::ProjectionLeftHand(fovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, nearZ, farZ);

	Renderer::Instance()->SetPorjectionCamera(view, projection);
}

//--------------------------------------------------------------------------------
//  カメラの描画範囲内に入ってるかどうか
//--------------------------------------------------------------------------------
bool Camera::IsInRange(const Vector3& position)
{ // Todo : カメラ前後判定、物体の範囲も判定、視野角の判定...
	return Vector3::SquareDistanceBetween(rig.Position, position) > farZ * farZ;
}

//--------------------------------------------------------------------------------
//  移動処理
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
//  コンストラクタ
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
//  カメラ前方と上と右を常に90度になってるを確保する
//--------------------------------------------------------------------------------
//void Camera::normalize(void)
//{
//	forward.Normalize();
//	up = (forward * right).Normalized();
//	right = (up * forward).Normalized();
//}

//--------------------------------------------------------------------------------
//  X軸回転
//--------------------------------------------------------------------------------
void Camera::pitch(const float& radian)
{
	pivot.Rotation.X += radian;
}

//--------------------------------------------------------------------------------
//  Y軸回転
//--------------------------------------------------------------------------------
void Camera::yaw(const float& radian)
{
	rig.Rotation.Y += radian;
}

//--------------------------------------------------------------------------------
//  パラメーターの算出
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
//  NormalCameraクラス
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
NormalCamera::NormalCamera() : Camera()
{
	CameraManager::Instance()->RegisterAsMain(this);
}
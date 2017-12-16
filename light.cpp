//--------------------------------------------------------------------------------
//　light.cpp
//	ライトクラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "light.h"
#include "main_system.h"
#include "shadow_map_system.h"
#include "camera.h"
#include "camera_manager.h"

//--------------------------------------------------------------------------------
//　ShadowMapLight::Set
//--------------------------------------------------------------------------------
void ShadowMapLight::Set(void)
{
	// 移動
	auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
	auto& movement = camera->GetWorldAtPosition() - look_at_;
	look_at_ += movement;
	position_ += movement;

	// View行列
	direction_ = (look_at_ - position_).Normalized();
	auto& right = (Vector3::kUp * direction_).Normalized();
	auto& up = (direction_ * right).Normalized();

	auto& negative_eye = position_ * -1.0f;
	auto& view_transpose = Matrix44(
		right.x_, right.y_, right.z_, right.Dot(negative_eye),
		up.x_, up.y_, up.z_, up.Dot(negative_eye),
		direction_.x_, direction_.y_, direction_.z_, direction_.Dot(negative_eye),
		0.0f, 0.0f, 0.0f, 1.0f);
	view_ = view_transpose.Transpose();

	// Projection行列
	//D3DXMATRIX ortho;
	//D3DXMatrixOrthoLH(&ortho, ShadowMapSystem::kShadowMapWidth, ShadowMapSystem::kShadowMapHeight, near_, far_);
	//projection_ = Matrix44::ToMatrix44(ortho);
	projection_ = Matrix44::ProjectionLeftHand(kPi * 0.3f
		, (float)ShadowMapSystem::kShadowMapWidth / ShadowMapSystem::kShadowMapHeight, near_, far_);
}
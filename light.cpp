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
	auto& forward = (look_at_ - position_).Normalized();
	auto& right = (Vector3::kUp * forward).Normalized();
	auto& up = (forward * right).Normalized();

	auto& negative_eye = position_ * -1.0f;
	auto& view_transpose = Matrix44(
		right.x_, right.y_, right.z_, right.Dot(negative_eye),
		up.x_, up.y_, up.z_, up.Dot(negative_eye),
		forward.x_, forward.y_, forward.z_, forward.Dot(negative_eye),
		0.0f, 0.0f, 0.0f, 1.0f);
	view_ = view_transpose.Transpose();

	// Projection行列
	projection_ = Matrix44::ProjectionLeftHand(kPi * 0.3f
		, (float)ShadowMapSystem::kShadowMapWidth / ShadowMapSystem::kShadowMapHeight, near_, far_);
}
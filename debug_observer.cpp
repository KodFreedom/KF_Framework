//--------------------------------------------------------------------------------
//　debug_observer.cpp
//	debug観察者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#ifdef _DEBUG
#include "debug_observer.h"
#include "ImGui\imgui.h"
#include "main_system.h"
#include "render_system.h"
#include "collision_system.h"
#include "mode.h"
#include "camera.h"
#include "camera_manager.h"

// shadowmap
#include "shadow_map_system.h"
#include "light_manager.h"
#include "light.h"

// player
#include "actor_observer.h"
#include "game_object.h"
#include "transform.h"
#include "player_controller.h"
#include "rigidbody3D.h"
#include "animator.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "ImGui\imgui_impl_dx9.h"
#endif

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void DebugObserver::Update(void)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void DebugObserver::LateUpdate(void)
{
	ShowMainWindow();
	ShowCollisionSystemWindow();
	ShowCameraWindow();
	ShowPlayerWindow();
	ShowFogWindow();
    ShowShadowMapWindow();
    ShowLogWindow();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DebugObserver::Render(void)
{
}

//--------------------------------------------------------------------------------
//  DebugLogに表示したい文字列を追加する
//--------------------------------------------------------------------------------
void DebugObserver::Display(const String& log)
{
	log_.append(log + L"\n");
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  DebugMainMenuの表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowMainWindow(void)
{
	auto render_system = MainSystem::Instance()->GetRenderSystem();
	auto background_color = render_system->GetBackgroundColor();

	// Begin
	if (!ImGui::Begin("Main Debug Window"))
	{
		ImGui::End();
		return;
	}

	// FPS
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	// BG Color
	if (ImGui::ColorEdit3("Background color", (float*)&background_color))
	{
		render_system->SetBackgroundColor(background_color);
	}

	// Window
	if (ImGui::Button(enable_collision_system_window_ ? "Close Collision System Window" : "Open Collision System Window")) enable_collision_system_window_ ^= 1;
	if (ImGui::Button(enable_camera_window_ ? "Close Camera Window" : "Open Camera Window")) enable_camera_window_ ^= 1;
	if (ImGui::Button(enable_player_window_ ? "Close Player Window" : "Open Player Window")) enable_player_window_ ^= 1;
    if (ImGui::Button(enable_shadow_map_system_window_ ? "Close Shadow Map Window" : "Open Shadow Map Window")) enable_shadow_map_system_window_ ^= 1;
    if (ImGui::Button(enable_log_window_ ? "Close Log Window" : "Open Log Window")) enable_log_window_ ^= 1;
	//if (ImGui::Button(enable_fog_window_ ? "Close Fog Window" : "Open Fog Window")) enable_fog_window_ ^= 1;

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  Collision情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowCollisionSystemWindow(void)
{
	if (!enable_collision_system_window_) return;
	//auto colliderSystem = CollisionSystem::Instance();

	// Begin
	if (!ImGui::Begin("Collision System Debug Window", &enable_collision_system_window_))
	{
		ImGui::End();
		return;
	}

	//// Draw Collider
	//ImGui::Checkbox("Show Collider", &pCS->m_bDrawCollider);

	//// Num Collider
	//if (ImGui::CollapsingHeader("Static"))
	//{
	//	ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[STATIC][Sphere].size());
	//	ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[STATIC][AABB].size());
	//	ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[STATIC][OBB].size());
	//}
	//if (ImGui::CollapsingHeader("CM_Dynamic"))
	//{
	//	ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[CM_Dynamic][Sphere].size());
	//	ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[CM_Dynamic][AABB].size());
	//	ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[CM_Dynamic][OBB].size());
	//}

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  Camera情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowCameraWindow(void)
{
	if (!enable_camera_window_) return;

	auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();

	// Begin
	if (!ImGui::Begin("Camera Window", &enable_camera_window_))
	{
		ImGui::End();
		return;
	}

	// Rig
	ImGui::Text("Rig position : %.3f, %.3f, %.3f", camera->rig_.position.x_, camera->rig_.position.y_, camera->rig_.position.z_);
	ImGui::Text("Rig rotation : %.3f, %.3f, %.3f", camera->rig_.rotation.x_, camera->rig_.rotation.y_, camera->rig_.rotation.z_);

	// Pivot
	ImGui::Text("Pivot position : %.3f, %.3f, %.3f", camera->pivot_.position.x_, camera->pivot_.position.y_, camera->pivot_.position.z_);
	ImGui::Text("Pivot rotation : %.3f, %.3f, %.3f", camera->pivot_.rotation.x_, camera->pivot_.rotation.y_, camera->pivot_.rotation.z_);
	
	// Eye,At
	ImGui::Text("World eye : %.3f, %.3f, %.3f", camera->world_eye_position_.x_, camera->world_eye_position_.y_, camera->world_eye_position_.z_);
	ImGui::Text("World at : %.3f, %.3f, %.3f", camera->world_at_position_.x_, camera->world_at_position_.y_, camera->world_at_position_.z_);

	// Right, Up, Forward
	ImGui::Text("World right : %.3f, %.3f, %.3f", camera->world_right_.x_, camera->world_right_.y_, camera->world_right_.z_);
	ImGui::Text("World up : %.3f, %.3f, %.3f", camera->world_up_.x_, camera->world_up_.y_, camera->world_up_.z_);
	ImGui::Text("World forward : %.3f, %.3f, %.3f", camera->world_forward_.x_, camera->world_forward_.y_, camera->world_forward_.z_);

	// Far, Near, Fov
	ImGui::InputFloat("Far", &camera->far_);
	ImGui::InputFloat("Near", &camera->near_);
	ImGui::InputFloat("Fov", &camera->fov_);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  Player情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowPlayerWindow(void)
{
    auto player = MainSystem::Instance()->GetActorObserver()->GetPlayer();
	if (!enable_player_window_ || !player) return;

	// Begin
	if (!ImGui::Begin("Player Window", &enable_player_window_))
	{
		ImGui::End();
		return;
	}

    if (ImGui::TreeNode("Transform"))
    {// Trans
        auto transform = player->GetGameObject().GetTransform();
        Vector3 position = transform->GetPosition();
        Quaternion rotation = transform->GetRotation();
        Vector3 scale = transform->GetScale();
        if (ImGui::InputFloat3("Position", &position.x_)) transform->SetPosition(position);
        if (ImGui::InputFloat4("Rotation", &rotation.x_)) transform->SetRotation(rotation);
        if (ImGui::InputFloat3("Scale", &scale.x_)) transform->SetScale(scale);
        ImGui::TreePop();
    }

    if (player)
    {
        if (ImGui::TreeNode("Parameter"))
        {// Parameter
            // State
            auto& state_name = player->GetCurrentStateName();
            ImGui::Text("State : %s", string(state_name.begin(), state_name.end()).c_str());

            // Animation
            auto& animation_name = player->GetAnimator().GetCurrentAnimationName();
            ImGui::Text("Animation : %s", string(animation_name.begin(), animation_name.end()).c_str());

            // Rigidbody
            const auto& rigidbody = player->GetRigidbody();
            ImGui::Text("Gravity multiplier : %.3f", rigidbody.GetGravityMultiplier());

            // Parameter
            auto& parameter = player->GetParameter();
            ImGui::Text("Ground check distance : %.3f", parameter.ground_check_distance_);
            ImGui::Text("Movement multiplier : %.3f", parameter.movement_multiplier_);
            ImGui::InputFloat("Move speed", &parameter.move_speed_);
            ImGui::InputFloat("Jump speed", &parameter.jump_speed_);
            ImGui::InputFloat("Min turn speed", &parameter.min_turn_speed_);
            ImGui::InputFloat("Max turn speed", &parameter.max_turn_speed_);
            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNode("Inverse kinematics"))
    {// IK
        auto& animator = player->GetAnimator();
        ImGui::SliderFloat("Ray distance", &animator.ik_ray_distance_, 0.0f, 2.0f);
        ImGui::SliderFloat("Grounded distance", &animator.ik_grounded_distance_, 0.0f, 1.0f);
        ImGui::InputFloat("Weight increase speed", &animator.ik_weight_increase_speed_);
        ImGui::InputFloat("Weight decrease speed", &animator.ik_weight_decrease_speed_);
        ImGui::InputFloat3("Foot position offset", &animator.ik_foot_position_offset_.x_);
        ImGui::InputFloat3("Foot rotation offset", &animator.ik_foot_rotation_offset_.x_);
        ImGui::TextColored(ImColor(0.0f, 1.0f, 0.0f), "Left foot ik goal");
        ImGui::Text("Position : %.3f, %.3f, %.3f", animator.ik_goals_[Animator::kIKGoalLeftFoot].position.x_, animator.ik_goals_[Animator::kIKGoalLeftFoot].position.y_, animator.ik_goals_[Animator::kIKGoalLeftFoot].position.z_);
        ImGui::Text("Up : %.3f, %.3f, %.3f", animator.ik_goals_[Animator::kIKGoalLeftFoot].up.x_, animator.ik_goals_[Animator::kIKGoalLeftFoot].up.y_, animator.ik_goals_[Animator::kIKGoalLeftFoot].up.z_);
        ImGui::Text("Weight : %.3f", animator.ik_goals_[Animator::kIKGoalLeftFoot].weight);
        ImGui::TextColored(ImColor(0.0f, 1.0f, 0.0f), "Right foot ik goal");
        ImGui::Text("Position : %.3f, %.3f, %.3f", animator.ik_goals_[Animator::kIKGoalRightFoot].position.x_, animator.ik_goals_[Animator::kIKGoalRightFoot].position.y_, animator.ik_goals_[Animator::kIKGoalRightFoot].position.z_);
        ImGui::Text("Up : %.3f, %.3f, %.3f", animator.ik_goals_[Animator::kIKGoalRightFoot].up.x_, animator.ik_goals_[Animator::kIKGoalRightFoot].up.y_, animator.ik_goals_[Animator::kIKGoalRightFoot].up.z_);
        ImGui::Text("Weight : %.3f", animator.ik_goals_[Animator::kIKGoalRightFoot].weight);
        ImGui::TreePop();
    }
   
	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  Fog情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowFogWindow(void)
{
	if (!enable_fog_window_) return;
	//auto pFog = Main::GetManager()->GetFog();

	// Begin
	if (!ImGui::Begin("Fog Window", &enable_fog_window_))
	{
		ImGui::End();
		return;
	}

	//// Enable
	//auto bEnable = pFog->GetEnable();
	//if (ImGui::Checkbox("Enable", &bEnable))
	//{
	//	pFog->SetEnable(bEnable);
	//}

	//// Enable RangeFog
	//auto bRangeFog = pFog->GetEnableRangeFog();
	//if (ImGui::Checkbox("RangeFog", &bRangeFog))
	//{
	//	pFog->SetEnableRangeFog(bRangeFog);
	//}

	//// Start
	//auto fStart = pFog->GetStart();
	//if (ImGui::InputFloat("Start", &fStart))
	//{
	//	pFog->SetStart(fStart);
	//}

	//// End
	//auto fEnd = pFog->GetEnd();
	//if (ImGui::InputFloat("End", &fEnd))
	//{
	//	pFog->SetEnd(fEnd);
	//}

	//// Density
	//auto fDensity = pFog->GetDensity();
	//if (ImGui::InputFloat("Density", &fDensity))
	//{
	//	pFog->SetDensity(fDensity);
	//}

	//// Color
	//auto cColor = pFog->GetColor();
	//if (ImGui::ColorEdit4("Color", (float*)&cColor))
	//{
	//	pFog->SetColor(cColor);
	//}

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  ShadowMap情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowShadowMapWindow(void)
{
    if (!enable_shadow_map_system_window_) return;

    // Begin
    if (!ImGui::Begin("Shadow Map Window", &enable_shadow_map_system_window_))
    {
        ImGui::End();
        return;
    }

    auto shadow_map_system = MainSystem::Instance()->GetShadowMapSystem();

    // Bias
    ImGui::InputFloat("Bias", &shadow_map_system->bias_);
    
    // Range
    ImGui::SliderFloat("Range", &shadow_map_system->range_, 0.0f, 50.0f);

    // Near
    ImGui::SliderFloat("Near", &shadow_map_system->near_, 0.0f, 10.0f);

    // Far
    ImGui::SliderFloat("Far", &shadow_map_system->far_, 50.0f, 500.0f);

    // Offset
    ImGui::InputFloat3("Offset", (float*)&shadow_map_system->offset_.x_);

    // Image
    ImGui::Image((void*)shadow_map_system->GetShadowMap(), ImVec2(128, 128)
        , ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  Log情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowLogWindow(void)
{
    if (!enable_log_window_) return;

    // Begin
    if (!ImGui::Begin("Log Window", &enable_log_window_))
    {
        ImGui::End();
        return;
    }

    string log(log_.begin(), log_.end());
    ImGui::Text(log.c_str());

    // End
    ImGui::End();
}
#endif//_DEBUG
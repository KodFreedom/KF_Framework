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
#include "collisionSystem.h"
#include "mode.h"
#include "camera.h"
#include "cameraManager.h"
#include "gameObjectActor.h"
#include "transform.h"
#include "playerController.h"

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
#ifndef EDITOR
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ImGui_ImplDX9_NewFrame();
#endif // USING_DIRECTX
#endif // !EDITOR
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
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DebugObserver::Render(void)
{
#ifndef EDITOR
	ImGui::Render();
#endif // !EDITOR
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
	if (ImGui::Button(enable_fog_window_ ? "Close Fog Window" : "Open Fog Window")) enable_fog_window_ ^= 1;

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

	auto camera = CameraManager::Instance()->GetMainCamera();

	// Begin
	if (!ImGui::Begin("Camera Window", &enable_camera_window_))
	{
		ImGui::End();
		return;
	}

	// Fov
	ImGui::InputFloat("Fov", &camera->fovY);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  Player情報の表示処理
//--------------------------------------------------------------------------------
void DebugObserver::ShowPlayerWindow(void)
{
	if (!enable_player_window_ || !player_) return;

	// Begin
	if (!ImGui::Begin("Player Window", &enable_player_window_))
	{
		ImGui::End();
		return;
	}

	// Trans
	auto transform = player_->GetTransform();
	ImGui::InputFloat3("Trans", &transform->nextPosition.x_);

	// Actor Behavior
	//auto pActor = static_cast<PlayerController*>(player->GetBehaviors().front());
	//ImGui::InputFloat("Move Speed", &pActor->m_fMoveSpeed);
	//ImGui::InputFloat("Jump Speed", &pActor->m_fJumpSpeed);
	//ImGui::Text("IsGrounded : %d", (int)pActor->m_bIsGrounded);

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
#endif//_DEBUG
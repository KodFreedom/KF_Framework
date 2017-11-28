//--------------------------------------------------------------------------------
//　main_system.cpp
//	メインシステム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "main_system.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "material_manager.h"
#include "motion_manager.h"
#include "light_manager.h"
#include "sound_manager.h"
#include "renderer_manager.h"
#include "input.h"
#include "fade_system.h"
#include "gameObjectManager.h"
#include "UISystem.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "collisionSystem.h"
#include "physicsSystem.h"
#include "cameraManager.h"
#include "camera.h"

#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#endif
#endif

#ifdef EDITOR
#include "modeEditor.h"
#endif // EDITOR

#ifdef _DEBUG
#include "debug_observer.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
MainSystem* MainSystem::instance_ = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  create the instance / 生成処理
//--------------------------------------------------------------------------------
MainSystem* MainSystem::Create(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode)
{
	if (instance_) return instance_;
	instance_ = new MainSystem;
	instance_->Init(hinstance, hwnd, is_window_mode);
	return instance_;
}

//--------------------------------------------------------------------------------
//	破棄処理
//--------------------------------------------------------------------------------
void MainSystem::Release(void)
{
	SAFE_UNINIT(instance_);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MainSystem::Update(void)
{
#ifdef _DEBUG
	debug_observer_->Update();
#endif
	input_->Update();
	current_mode_->Update();
	GameObjectManager::Instance()->Update();
	CollisionSystem::Instance()->Update();
	PhysicsSystem::Instance()->Update();
	CameraManager::Instance()->Update();
}

//--------------------------------------------------------------------------------
//  後更新処理(描画の前)
//--------------------------------------------------------------------------------
void MainSystem::LateUpdate(void)
{
	current_mode_->LateUpdate();
	GameObjectManager::Instance()->LateUpdate();
	CameraManager::Instance()->LateUpdate();
	CollisionSystem::Instance()->LateUpdate();
	UISystem::Instance()->Update();
	fade_system_->Update();
	renderer_manager_->Update();
#ifdef _DEBUG
	debug_observer_->LateUpdate();
#endif
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void MainSystem::Render(void)
{
	if (render_system_->BeginRender())
	{
		CameraManager::Instance()->GetMainCamera()->Set();
		renderer_manager_->Render();
#ifdef _DEBUG
		CollisionSystem::Instance()->DrawCollider();
#endif
		UISystem::Instance()->Draw();
		fade_system_->Render();
#ifdef _DEBUG
		debug_observer_->Render();
#endif
		render_system_->EndRender();
		render_system_->Present();
	}
	else
	{
		renderer_manager_->Clear();
	}
}

//--------------------------------------------------------------------------------
//  モード切り替え
//--------------------------------------------------------------------------------
void MainSystem::Change(Mode* next_mode)
{
	SAFE_RELEASE(current_mode_);
	current_mode_ = next_mode;
	current_mode_->Init();
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool MainSystem::Init(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode)
{
	random::Init();

	// render apiによってrender system, texture manager, mesh managerの生成
#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
	auto render_system_directX9 = RenderSystemDirectX9::Create(hwnd, is_window_mode);
	if (!render_system_directX9) return false;
	render_system_ = render_system_directX9;
	const auto device = render_system_directX9->GetDevice();
	texture_manager_ = TextureManager::Create(device);
	mesh_manager_ = MeshManager::Create(device);
#endif
#endif
	material_manager_ = MaterialManager::Create();
	motion_manager_ = MotionManager::Create();
	light_manager_ = LightManager::Create();
	sound_manager_ = SoundManager::Create();
	renderer_manager_ = RendererManager::Create();
#ifdef _DEBUG
	debug_observer_ = DebugObserver::Create();
#endif
	input_ = Input::Create(hinstance, hwnd);
	CollisionSystem::Create();
	PhysicsSystem::Create();
	GameObjectManager::Create();
	UISystem::Create();
	FadeSystem::Create();
	CameraManager::Create();

	//初期モード設定
#ifdef EDITOR
	Change(new ModeEditor);
#else
	Change(new ModeDemo);
#endif // EDITOR

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MainSystem::Uninit(void)
{
	SAFE_RELEASE(current_mode_);
	CameraManager::Release();
	UISystem::Release();
	GameObjectManager::Release();
	PhysicsSystem::Release();
	CollisionSystem::Release();
	SAFE_RELEASE(fade_system_);
	SAFE_RELEASE(input_);
#ifdef _DEBUG
	SAFE_RELEASE(debug_observer_);
#endif
	SAFE_RELEASE(renderer_manager_);
	SAFE_RELEASE(sound_manager_);
	SAFE_RELEASE(light_manager_);
	SAFE_RELEASE(motion_manager_);
	SAFE_RELEASE(material_manager_);
	SAFE_RELEASE(mesh_manager_);
	SAFE_RELEASE(texture_manager_);
	SAFE_RELEASE(render_system_);
}
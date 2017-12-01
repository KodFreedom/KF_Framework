//--------------------------------------------------------------------------------
//　main_system.cpp
//	メインシステム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
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
#include "camera_manager.h"
#include "ui_system.h"
#include "collision_system.h"
#include "physics_system.h"
#include "game_object_manager.h"
#include "shader_manager.h"
#include "mode_title.h"
#include "mode_demo.h"

#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#endif
#endif

#ifdef EDITOR
#include "mode_editor.h"
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
	instance_ = MY_NEW MainSystem;
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
	game_object_manager_->Update();
	collision_system_->Update();
	physics_system_->Update();
	camera_manager_->Update();
}

//--------------------------------------------------------------------------------
//  後更新処理(描画の前)
//--------------------------------------------------------------------------------
void MainSystem::LateUpdate(void)
{
	current_mode_->LateUpdate();
	game_object_manager_->LateUpdate();
	camera_manager_->LateUpdate();
	collision_system_->LateUpdate();
	ui_system_->Update();
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
		camera_manager_->SetCamera();
		renderer_manager_->Render();
#ifdef _DEBUG
		collision_system_->Render();
#endif
		ui_system_->Render();
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
	Random::Init();

	// render apiによってrender system, texture manager, mesh managerの生成
#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
	auto render_system_directX9 = RenderSystemDirectX9::Create(hwnd, is_window_mode);
	if (!render_system_directX9) return false;
	render_system_ = render_system_directX9;
	const auto device = render_system_directX9->GetDevice();
	texture_manager_ = TextureManager::Create(device);
	mesh_manager_ = MeshManager::Create(device);
	shader_manager_ = ShaderManager::Create(device);
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
	fade_system_ = FadeSystem::Create();
	camera_manager_ = CameraManager::Create();
	ui_system_ = UISystem::Create();
	collision_system_ = CollisionSystem::Create();
	physics_system_ = PhysicsSystem::Create();
	game_object_manager_ = GameObjectManager::Create();

	//初期モード設定
#ifdef EDITOR
	Change(MY_NEW ModeEditor);
#else
	Change(MY_NEW ModeDemo);
#endif // EDITOR

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MainSystem::Uninit(void)
{
	SAFE_RELEASE(current_mode_);
	SAFE_RELEASE(game_object_manager_);
	SAFE_RELEASE(physics_system_);
	SAFE_RELEASE(collision_system_);
	SAFE_RELEASE(ui_system_);
	SAFE_RELEASE(camera_manager_);
	SAFE_RELEASE(fade_system_);
	SAFE_RELEASE(input_);
#ifdef _DEBUG
	SAFE_RELEASE(debug_observer_);
#endif
	SAFE_RELEASE(shader_manager_);
	SAFE_RELEASE(renderer_manager_);
	SAFE_RELEASE(sound_manager_);
	SAFE_RELEASE(light_manager_);
	SAFE_RELEASE(motion_manager_);
	SAFE_RELEASE(material_manager_);
	SAFE_RELEASE(mesh_manager_);
	SAFE_RELEASE(texture_manager_);
	SAFE_RELEASE(render_system_);
}
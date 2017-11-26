//--------------------------------------------------------------------------------
//　main_system.cpp
//	メインシステム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "main_system.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "rendererManager.h"
#include "input.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "soundManager.h"
#include "UISystem.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "fadeSystem.h"
#include "collisionSystem.h"
#include "physicsSystem.h"
#include "cameraManager.h"
#include "camera.h"
#include "motionManager.h"

#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#endif
#endif

#if defined(_DEBUG) || defined(EDITOR)
#include "modeEditor.h"
#include "debugObserver.h"
#endif

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
#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Instance()->Update();
#endif
	Input::Instance()->Update();
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
	FadeSystem::Instance()->Update();
	RendererManager::Instance()->Update();
#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Instance()->LateUpdate();
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
		RendererManager::Instance()->Render();
#ifdef _DEBUG
		CollisionSystem::Instance()->DrawCollider();
#endif
		UISystem::Instance()->Draw();
		FadeSystem::Instance()->Draw();
#if defined(_DEBUG) || defined(EDITOR)
		DebugObserver::Instance()->Draw();
#endif
		render_system_->EndRender();
		render_system_->Present();
	}
	else
	{
		RendererManager::Instance()->Clear();
	}
}

//--------------------------------------------------------------------------------
//  モード切り替え
//--------------------------------------------------------------------------------
void MainSystem::Change(Mode* nextMode)
{
	SAFE_RELEASE(current_mode_);
	current_mode_ = nextMode;
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
	auto render_system = RenderSystemDirectX9::Create(hwnd, is_window_mode);
	if (!render_system) return false;
	render_system_ = render_system;
	const auto device = render_system->GetDevice();
	texture_manager_ = TextureManager::Create(device);
	mesh_manager_ = MeshManager::Create(device);
	
#endif
#endif

#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Create(hwnd);
#endif
	RendererManager::Create();
	Input::Create(hinstance, hwnd);
	MaterialManager::Create();
	CollisionSystem::Create();
	PhysicsSystem::Create();
	GameObjectManager::Create();
	UISystem::Create();
	SoundManager::Create();
	FadeSystem::Create();
	CameraManager::Create();
	MotionManager::Create();

	//初期モード設定
	Change(new ModeDemo);

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MainSystem::Uninit(void)
{
	SAFE_RELEASE(current_mode_);
	MotionManager::Release();
	CameraManager::Release();
	FadeSystem::Release();
	SoundManager::Release();
	UISystem::Release();
	GameObjectManager::Release();
	PhysicsSystem::Release();
	CollisionSystem::Release();
	MaterialManager::Release();
	Input::Release();
#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Release();
#endif
	RendererManager::Release();
	SAFE_RELEASE(mesh_manager_);
	SAFE_RELEASE(texture_manager_);
	SAFE_RELEASE(render_system_);
}
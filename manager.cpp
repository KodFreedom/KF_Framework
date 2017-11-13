//--------------------------------------------------------------------------------
//
//　manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderSystem.h"
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

#if defined(_DEBUG) || defined(EDITOR)
#include "modeEditor.h"
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
Manager* Manager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：Create
//  関数説明：生成処理
//	引数：	hInstance：値
//			hWnd：
//			isWindowMode：
//	戻り値：Manager*
//--------------------------------------------------------------------------------
Manager* Manager::Create(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode)
{
	if (instance) return instance;
	instance = new Manager;
	instance->init(hInstance, hWnd, isWindowMode);
	return instance;
}

//--------------------------------------------------------------------------------
//	関数名：Release
//  関数説明：破棄処理
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Manager::Release(void)
{
	SAFE_UNINIT(instance);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Manager::Update(void)
{
#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Instance()->Update();
#endif
	Input::Instance()->Update();
	currentMode->Update();
	GameObjectManager::Instance()->Update();
	CollisionSystem::Instance()->Update();
	PhysicsSystem::Instance()->Update();
	CameraManager::Instance()->Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void Manager::LateUpdate(void)
{
	currentMode->LateUpdate();
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
void Manager::Render(void)
{
	if (RenderSystem::Instance()->BeginRender())
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
		RenderSystem::Instance()->EndRender();
	}
}

//--------------------------------------------------------------------------------
//  モード切り替え
//--------------------------------------------------------------------------------
void Manager::Change(Mode* nextMode)
{
	SAFE_RELEASE(currentMode);
	currentMode = nextMode;
	currentMode->Init();
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool Manager::init(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode)
{
	Random::Init();
	if (!RenderSystem::Create(hWnd, isWindowMode)) return false;
#if defined(_DEBUG) || defined(EDITOR)
	DebugObserver::Create(hWnd);
#endif
	RendererManager::Create();
	Input::Create(hInstance, hWnd);
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
void Manager::uninit(void)
{
	SAFE_RELEASE(currentMode);
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
	RenderSystem::Release();
}
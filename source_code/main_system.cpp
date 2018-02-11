//--------------------------------------------------------------------------------
//　main_system.cpp
//  メインシステム
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main_system.h"
#include "resources.h"
#include "light_manager.h"
#include "sound_system.h"
#include "renderer_manager.h"
#include "input.h"
#include "fade_system.h"
#include "camera_manager.h"
#include "collision_system.h"
#include "physics_system.h"
#include "game_object_manager.h"
#include "shader_manager.h"
#include "shadow_map_system.h"
#include "actor_observer.h"
#include "mode_title.h"
#include "mode_demo.h"
#include "mode_demo_play.h"

#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
#include "render_system_directX9.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "ImGui\imgui_impl_dx9.h"
#endif
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
//  破棄処理
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
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    ImGui_ImplDX9_NewFrame();
#endif
#endif
#ifdef _DEBUG
    debug_observer_->Update();
#endif
    fade_system_->Update();
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
    // 事前セッティング
    camera_manager_->SetCamera();

    // shadowmap
    shadow_map_system_->Render();

    // backbuffer
    if (render_system_->BeginRender())
    {
        renderer_manager_->Render();
#ifdef _DEBUG
        collision_system_->Render();
        debug_observer_->Render();
#endif
#if defined(_DEBUG) || defined(EDITOR)
        ImGui::Render();
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

    // render apiによってrender system, resourcesの生成
#if defined(USING_DIRECTX)
#if (DIRECTX_VERSION == 9)
    auto render_system_directX9 = RenderSystemDirectX9::Create(hwnd, is_window_mode);
    if (!render_system_directX9) return false;
    render_system_ = render_system_directX9;
    const auto device = render_system_directX9->GetDevice();

    resources_ = Resources::Create(device);
    shader_manager_ = ShaderManager::Create(device);
    shadow_map_system_ = ShadowMapSystem::Create(device);
#endif
#endif

#if defined(_DEBUG) || defined(EDITOR)
    // Imgui font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    io.ImeWindowHandle = hwnd;
#endif

    light_manager_ = LightManager::Create();
    renderer_manager_ = RendererManager::Create();
#ifdef _DEBUG
    debug_observer_ = DebugObserver::Create();
#endif
    sound_system_ = SoundSystem::Create();
    input_ = Input::Create(hinstance, hwnd);
    camera_manager_ = CameraManager::Create();
    collision_system_ = CollisionSystem::Create();
    physics_system_ = PhysicsSystem::Create();
    game_object_manager_ = GameObjectManager::Create();
    fade_system_ = FadeSystem::Create();
    actor_observer_ = ActorObserver::Create();

    //初期モード設定
#ifdef EDITOR
    Change(MY_NEW ModeTitle);
#else
    Change(MY_NEW ModeTitle);
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
    SAFE_RELEASE(actor_observer_);
    SAFE_RELEASE(physics_system_);
    SAFE_RELEASE(collision_system_);
    SAFE_RELEASE(camera_manager_);
    SAFE_RELEASE(fade_system_);
    SAFE_RELEASE(input_);
    SAFE_RELEASE(sound_system_);
#ifdef _DEBUG
    SAFE_RELEASE(debug_observer_);
#endif
    SAFE_RELEASE(shadow_map_system_);
    SAFE_RELEASE(shader_manager_);
    SAFE_RELEASE(renderer_manager_);
    SAFE_RELEASE(light_manager_);
    SAFE_RELEASE(resources_);
    SAFE_RELEASE(render_system_);
}
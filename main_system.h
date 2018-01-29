//--------------------------------------------------------------------------------
//　main_system.h
//  メインシステム、ベースシステムを管理する
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//--------------------------------------------------------------------------------
//  forward declaration / 前方宣言
//--------------------------------------------------------------------------------
class Mode;
class RenderSystem;
class RendererManager;
class Resources;
class Input;
class CollisionSystem;
class PhysicsSystem;
class GameObjectManager;
class SoundSystem;
class FadeSystem;
class CameraManager;
class LightManager;
class ShaderManager;
class ShadowMapSystem;
class ActorObserver;
#if defined(_DEBUG)
class DebugObserver;
#endif

//--------------------------------------------------------------------------------
//  メインシステムクラス
//--------------------------------------------------------------------------------
class MainSystem
{
public:
    //--------------------------------------------------------------------------------
    //  create the instance / 生成処理
    //  hinstance
    //  hwnd
    //  is_window_mode
    //  return：MainSystem*
    //--------------------------------------------------------------------------------
    static MainSystem* Create(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
    
    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    static void Release(void);

    //--------------------------------------------------------------------------------
    //  インスタンスを返す
    //--------------------------------------------------------------------------------
    static MainSystem& Instance(void) { return *instance_; }

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  後更新処理(描画の前)
    //--------------------------------------------------------------------------------
    void LateUpdate(void);

    //--------------------------------------------------------------------------------
    //  描画処理
    //--------------------------------------------------------------------------------
    void Render(void);

    //--------------------------------------------------------------------------------
    //  change the mode / モード切り替え処理
    //--------------------------------------------------------------------------------
    void Change(Mode* next_mode);

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    auto& GetRenderSystem(void) const { return *render_system_; }
    auto& GetResources(void) const { return *resources_; }
    auto& GetLightManager(void) const { return *light_manager_; }
    auto& GetSoundSystem(void) const { return *sound_system_; }
    auto& GetRendererManager(void) const { return *renderer_manager_; }
    auto& GetShaderManager(void) const { return *shader_manager_; }
    auto& GetInput(void) const { return *input_; }
    auto& GetFadeSystem(void) const { return *fade_system_; }
    auto& GetCameraManager(void) const { return *camera_manager_; }
    auto& GetCollisionSystem(void) const { return *collision_system_; }
    auto& GetPhysicsSystem(void) const { return *physics_system_; }
    auto& GetGameObjectManager(void) const { return *game_object_manager_; }
    auto& GetShadowMapSystem(void) const { return *shadow_map_system_; }
    auto& GetActorObserver(void) const { return *actor_observer_; }
    auto& GetCurrentMode(void) const { return *current_mode_; }
#ifdef _DEBUG
    auto& GetDebugObserver(void) const { return *debug_observer_; }
#endif

private:
    //--------------------------------------------------------------------------------
    //  constructors for singleton / シングルトンのコンストラクタ
    //--------------------------------------------------------------------------------
    MainSystem() {}
    MainSystem(const MainSystem& value) {}
    MainSystem& operator=(const MainSystem& value) {}
    ~MainSystem() {}

    //--------------------------------------------------------------------------------
    //  init the main system, return ture if is successed
    //  初期化処理
    //  hinstance
    //  hwnd
    //  is_window_mode
    //  return：bool
    //--------------------------------------------------------------------------------
    bool Init(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
    
    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Mode*              current_mode_ = nullptr;        // 今のモード
    RenderSystem*      render_system_ = nullptr;       // レンダーシステム
    Resources*         resources_ = nullptr;           // リソース統括者
    SoundSystem*       sound_system_ = nullptr;        // サウンドシステム
    LightManager*      light_manager_ = nullptr;       // ライトの管理者
    RendererManager*   renderer_manager_ = nullptr;    // レンダラー管理者
    ShaderManager*     shader_manager_ = nullptr;      // シェーダ管理者
#ifdef _DEBUG
    DebugObserver*     debug_observer_ = nullptr;      // デバッグ観察者
#endif
    Input*             input_ = nullptr;               // 入力管理者
    FadeSystem*        fade_system_ = nullptr;         // フェイドシステム
    CameraManager*     camera_manager_ = nullptr;      // カメラ管理者
    CollisionSystem*   collision_system_ = nullptr;    // 衝突処理システム
    PhysicsSystem*     physics_system_ = nullptr;      // 物理システム
    GameObjectManager* game_object_manager_ = nullptr; // ゲームオブジェクト管理者
    ShadowMapSystem*   shadow_map_system_ = nullptr;   // シャドウマップシステム
    ActorObserver*     actor_observer_ = nullptr;      // アクター観察者
    static MainSystem* instance_;                      // インスタンス
};
//--------------------------------------------------------------------------------
//　main_system.h
//	メインシステム、ベースシステムを管理する
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//--------------------------------------------------------------------------------
//  forward declaration / 前方宣言
//--------------------------------------------------------------------------------
class Mode;
class RenderSystem;
class RendererManager;
class Input;
class MaterialManager;
class CollisionSystem;
class PhysicsSystem;
class GameObjectManager;
class SoundManager;
class FadeSystem;
class CameraManager;
class MotionManager;
class TextureManager;
class MeshManager;
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
	//	hinstance
	//	hwnd
	//	is_window_mode
	//	return：MainSystem*
	//--------------------------------------------------------------------------------
	static MainSystem* Create(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
	
	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	static void Release(void);

	//--------------------------------------------------------------------------------
	//  return the instance of main system / インスタンスを返す
	//--------------------------------------------------------------------------------
	static auto Instance(void) { return instance_; }

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
	const auto GetRenderSystem(void) const { return render_system_; }
	const auto GetTextureManager(void) const { return texture_manager_; }
	const auto GetMeshManager(void) const { return mesh_manager_; }
	const auto GetMaterialManager(void) const { return material_manager_; }
	const auto GetMotionManager(void) const { return motion_manager_; }
	const auto GetLightManager(void) const { return light_manager_; }
	const auto GetSoundManager(void) const { return sound_manager_; }
	const auto GetRendererManager(void) const { return renderer_manager_; }
	const auto GetShaderManager(void) const { return shader_manager_; }
    const auto GetInput(void) const { return input_; }
    const auto GetFadeSystem(void) const { return fade_system_; }
    const auto GetCameraManager(void) const { return camera_manager_; }
    const auto GetCollisionSystem(void) const { return collision_system_; }
    const auto GetPhysicsSystem(void) const { return physics_system_; }
    const auto GetGameObjectManager(void) const { return game_object_manager_; }
    const auto GetShadowMapSystem(void) const { return shadow_map_system_; }
    const auto GetActorObserver(void) const { return actor_observer_; }
#ifdef _DEBUG
	const auto GetDebugObserver(void) const { return debug_observer_; }
#endif

private:
	//--------------------------------------------------------------------------------
	//  constructors for singleton / シングルトンのコンストラクタ
	//--------------------------------------------------------------------------------
	MainSystem() : current_mode_(nullptr), render_system_(nullptr)
		, texture_manager_(nullptr), mesh_manager_(nullptr)
		, material_manager_(nullptr), motion_manager_(nullptr)
		, light_manager_(nullptr), sound_manager_(nullptr)
		, renderer_manager_(nullptr), input_(nullptr)
#ifdef _DEBUG
		, debug_observer_(nullptr)
#endif
		, fade_system_(nullptr), camera_manager_(nullptr)
		, collision_system_(nullptr), physics_system_(nullptr)
        , game_object_manager_(nullptr), shader_manager_(nullptr)
        , shadow_map_system_(nullptr), actor_observer_(nullptr) {}
	MainSystem(const MainSystem& value) {}
	MainSystem& operator=(const MainSystem& value) {}
	~MainSystem() {}

	//--------------------------------------------------------------------------------
	//  init the main system, return ture if is successed
	//  初期化処理
	//	hinstance
	//	hwnd
	//	is_window_mode
	//	return：bool
	//--------------------------------------------------------------------------------
	bool Init(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
	
	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Mode*              current_mode_; // 今のモード
	RenderSystem*      render_system_; // レンダーシステム
	TextureManager*    texture_manager_; // テクスチャ管理者
	MeshManager*       mesh_manager_; // メッシュ管理者
	MaterialManager*   material_manager_; // マテリアル管理者
	MotionManager*     motion_manager_; // モーション管理者
	LightManager*      light_manager_; // ライトの管理者
	SoundManager*      sound_manager_; // サウンド管理者
	RendererManager*   renderer_manager_; // レンダラー管理者
	ShaderManager*     shader_manager_; // シェーダ管理者
#ifdef _DEBUG
	DebugObserver*     debug_observer_; // デバッグ観察者
#endif
	Input*             input_; // 入力管理者
	FadeSystem*        fade_system_; // フェイドシステム
	CameraManager*     camera_manager_; // カメラ管理者
	CollisionSystem*   collision_system_; // 衝突処理システム
	PhysicsSystem*     physics_system_; // 物理システム
	GameObjectManager* game_object_manager_; // ゲームオブジェクト管理者
	ShadowMapSystem*   shadow_map_system_; // シャドウマップシステム
    ActorObserver*     actor_observer_;// アクター観察者

	static MainSystem* instance_; // インスタンス
};
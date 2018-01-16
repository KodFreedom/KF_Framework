//--------------------------------------------------------------------------------
//	プレイヤーUIコントローラ
//　player_ui_controller.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_ui_controller.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "main_system.h"
#include "actor_observer.h"
#include "player_controller.h"
#include "material_manager.h"
#include "mesh_renderer_2d.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
const Vector3 PlayerUiController::kLifeGaugeSize = Vector3(540.0f, 33.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kLifeGaugeLeftTop = Vector3(-673.0f, 420.0f, 0.0f) * SCREEN_RATE;
const Color   PlayerUiController::kLifeGaugeMaxColor = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color   PlayerUiController::kLifeGaugeMinColor = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Vector3 PlayerUiController::kCoverSize = Vector3(804.0f, 179.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kCoverLeftTop = Vector3(-925.0f, 343.0f, 0.0f) * SCREEN_RATE;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructors and destructors
//--------------------------------------------------------------------------------
PlayerUiController::PlayerUiController(GameObject& owner)
    : Behavior(owner, L"PlayerUiController")
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool PlayerUiController::Init(void)
{
    // Life Gauge
    auto life_gauge = GameObjectSpawner::CreateBasicPolygon2d(
        kLifeGaugeSize,
        kDefaultLayer,
        L"life_gauge",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kLifeGaugeLeftTop + kLifeGaugeSize * 0.5f);
    life_gauge_ = life_gauge->GetTransform();
    
    // Cover
    GameObjectSpawner::CreateBasicPolygon2d(
        kCoverSize,
        kDefaultLayer,
        L"life_gauge_cover",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kCoverLeftTop + kCoverSize * 0.5f);

    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerUiController::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerUiController::Update(void)
{
    auto player = MainSystem::Instance()->GetActorObserver()->GetPlayer();
    if (!player) return;
    
    // Life gauge position and size
    const auto& parameter = player->GetParameter();
    float life_rate = parameter.GetCurrentLife() / parameter.GetMaxLife();
    Vector3 current_size = kLifeGaugeSize;
    current_size.x_ *= life_rate;
    Vector3& current_center = kLifeGaugeLeftTop + current_size * 0.5f;
    life_gauge_->SetPosition(current_center);
    life_gauge_->SetScale(current_size);

    // Life gauge color
    Color& current_color = Math::Lerp(kLifeGaugeMinColor, kLifeGaugeMaxColor, life_rate);
    auto life_gauge_material = MainSystem::Instance()->GetMaterialManager()->GetMaterial(L"life_gauge");
    life_gauge_material->diffuse_ = current_color;

    // Life gauge uv
    auto renderer = life_gauge_->GetGameObject().GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    ((MeshRenderer2d*)renderer)->SetUvScale(Vector2(life_rate, 1.0f));
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void PlayerUiController::LateUpdate(void)
{

}
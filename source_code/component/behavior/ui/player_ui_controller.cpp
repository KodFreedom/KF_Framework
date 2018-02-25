//--------------------------------------------------------------------------------
//  プレイヤーUIコントローラ
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_ui_controller.h"
#include "../../../game_object/game_object_spawner.h"
#include "../../../game_object/game_object.h"
#include "../../../main_system.h"
#include "../../../observer/actor_observer.h"
#include "../actor/player_controller.h"
#include "../../../resources/resources.h"
#include "../../../resources/material_manager.h"
#include "../../renderer/mesh_renderer_2d.h"
#include "../../../game_time.h"
#include "../../../input/input.h"
#include "../../../input/input_device_directX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
const Vector3 PlayerUiController::kLifeGaugeSize = Vector3(540.0f, 33.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kLifeGaugeLeftTop = Vector3(-923.0f, -500.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kCoverSize = Vector3(562.0f, 113.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kCoverLeftTop = Vector3(-935.0f, -520.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kButtonSize[kButtonTypeMax] =
{
    Vector3(160.0f, 40.0f, 0.0f) * SCREEN_RATE,
    Vector3(40.0f, 40.0f, 0.0f) * SCREEN_RATE,
};

// Jump
const Vector3 PlayerUiController::kButtonJumpLeftTop[kButtonTypeMax] =
{
    Vector3(750.0f, 305.0f, 0.0f) * SCREEN_RATE,
    Vector3(750.0f, 300.0f, 0.0f) * SCREEN_RATE,
};
const Vector2 PlayerUiController::kButtonJumpUvOffset[kButtonTypeMax] =
{
    Vector2(0.0f, 0.0f),
    Vector2(1.0f / 3.0f, 0.0f),
};
const Vector2 PlayerUiController::kButtonJumpUvScale[kButtonTypeMax] =
{
    Vector2(1.0f, 0.25f),
    Vector2(1.0f / 3.0f, 0.25f),
};

// LightAttack
const Vector3 PlayerUiController::kButtonLightAttackLeftTop[kButtonTypeMax] =
{
    Vector3(750.0f, 355.0f, 0.0f) * SCREEN_RATE,
    Vector3(750.0f, 350.0f, 0.0f) * SCREEN_RATE,
};
const Vector2 PlayerUiController::kButtonLightAttackUvOffset[kButtonTypeMax] =
{
    Vector2(0.0f, 0.25f),
    Vector2(2.0f / 3.0f, 0.0f),
};
const Vector2 PlayerUiController::kButtonLightAttackUvScale[kButtonTypeMax] =
{
    Vector2(1.0f, 0.25f),
    Vector2(1.0f / 3.0f, 0.25f),
};

// StrongAttack
const Vector3 PlayerUiController::kButtonStrongAttackLeftTop[kButtonTypeMax] =
{
    Vector3(750.0f, 405.0f, 0.0f) * SCREEN_RATE,
    Vector3(750.0f, 400.0f, 0.0f) * SCREEN_RATE,
};
const Vector2 PlayerUiController::kButtonStrongAttackUvOffset[kButtonTypeMax] =
{
    Vector2(0.0f, 0.5f),
    Vector2(0.0f, 0.25f),
};
const Vector2 PlayerUiController::kButtonStrongAttackUvScale[kButtonTypeMax] =
{
    Vector2(1.0f, 0.25f),
    Vector2(1.0f / 3.0f, 0.25f),
};

// Guard
const Vector3 PlayerUiController::kButtonGuardLeftTop[kButtonTypeMax] =
{
    Vector3(750.0f, 455.0f, 0.0f) * SCREEN_RATE,
    Vector3(750.0f, 450.0f, 0.0f) * SCREEN_RATE,
};
const Vector2 PlayerUiController::kButtonGuardUvOffset[kButtonTypeMax] =
{
    Vector2(0.0f, 0.75f),
    Vector2(1.0f / 3.0f, 0.75f),
};
const Vector2 PlayerUiController::kButtonGuardUvScale[kButtonTypeMax] =
{
    Vector2(1.0f, 0.25f),
    Vector2(1.0f / 3.0f, 0.25f),
};

// Texts
const Vector3 PlayerUiController::kButtonTextSize = Vector3(320.0f, 40.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kButtonTextJumpLeftTop = Vector3(410.0f, 305.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kButtonTextLightAttackLeftTop = Vector3(410.0f, 355.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kButtonTextStrongAttackLeftTop = Vector3(410.0f, 405.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kButtonTextGuardLeftTop = Vector3(410.0f, 455.0f, 0.0f) * SCREEN_RATE;

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
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool PlayerUiController::Init(void)
{
    auto& material_manager = MainSystem::Instance().GetResources().GetMaterialManager();

    // Warning Gauge
    auto warning_gauge = GameObjectSpawner::CreateBasicPolygon2d(
        kLifeGaugeSize,
        kDefaultLayer,
        L"warning_gauge",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kLifeGaugeLeftTop + kLifeGaugeSize * 0.5f);
    warning_gauge_ = warning_gauge->GetTransform();

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

    // Buttons
    ButtonType current_type = MainSystem::Instance().GetInput().GetJoystick()->IsAttached() ? kJoystickButton : kKeyboardButton;
    String button_texture[kButtonTypeMax] = {
        L"button_keyboard",
        L"button"};

    // Jump
    auto ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonSize[current_type],
        kDefaultLayer,
        button_texture[current_type],
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonJumpLeftTop[current_type] + kButtonSize[current_type] * 0.5f);
    auto renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    auto renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(kButtonJumpUvOffset[current_type]);
    renderer2d->SetUvScale(kButtonJumpUvScale[current_type]);

    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonTextSize,
        kDefaultLayer,
        L"button_text",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonTextJumpLeftTop + kButtonTextSize * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(Vector2(0.0f, 0.0f));
    renderer2d->SetUvScale(Vector2(1.0f, 0.25f));

    // LightAttack
    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonSize[current_type],
        kDefaultLayer,
        button_texture[current_type],
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonLightAttackLeftTop[current_type] + kButtonSize[current_type] * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(kButtonLightAttackUvOffset[current_type]);
    renderer2d->SetUvScale(kButtonLightAttackUvScale[current_type]);

    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonTextSize,
        kDefaultLayer,
        L"button_text",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonTextLightAttackLeftTop + kButtonTextSize * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(Vector2(0.0f, 0.25f));
    renderer2d->SetUvScale(Vector2(1.0f, 0.25f));

    // StrongAttack
    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonSize[current_type],
        kDefaultLayer,
        button_texture[current_type],
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonStrongAttackLeftTop[current_type] + kButtonSize[current_type] * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(kButtonStrongAttackUvOffset[current_type]);
    renderer2d->SetUvScale(kButtonStrongAttackUvScale[current_type]);

    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonTextSize,
        kDefaultLayer,
        L"button_text",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonTextStrongAttackLeftTop + kButtonTextSize * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(Vector2(0.0f, 0.5f));
    renderer2d->SetUvScale(Vector2(1.0f, 0.25f));

    // Guard
    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonSize[current_type],
        kDefaultLayer,
        button_texture[current_type],
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonGuardLeftTop[current_type] + kButtonSize[current_type] * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(kButtonGuardUvOffset[current_type]);
    renderer2d->SetUvScale(kButtonGuardUvScale[current_type]);

    ui = GameObjectSpawner::CreateBasicPolygon2d(
        kButtonTextSize,
        kDefaultLayer,
        L"button_text",
        kDefault2dTextureShader,
        k2d,
        0.0f,
        kButtonTextGuardLeftTop + kButtonTextSize * 0.5f);
    renderer = ui->GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    renderer2d = (MeshRenderer2d*)renderer;
    renderer2d->SetUvOffset(Vector2(0.0f, 0.75f));
    renderer2d->SetUvScale(Vector2(1.0f, 0.25f));

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
    auto player = MainSystem::Instance().GetActorObserver().GetPlayer();
    if (!player) return;
    const auto& parameter = player->GetParameter();
    UpdateWarningGauge(parameter);
    UpdateLifeGauge(parameter);
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void PlayerUiController::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ワーニングゲージの更新
//--------------------------------------------------------------------------------
void PlayerUiController::UpdateWarningGauge(const ActorParameter& parameter)
{
    auto warning_gauge_material = MainSystem::Instance().GetResources().GetMaterialManager().Get(L"warning_gauge");
    if (!warning_gauge_material) return;
    warning_gauge_material->diffuse_ = Color(0.5f, 0.2f, 0.2f, warning_gauge_material->diffuse_.a_);

    if (parameter.GetCurrentLife() >= parameter.GetMaxLife() * 0.5f)
    {
        warning_gauge_material->diffuse_.a_ = 0.0f;
        return;
    }

    // 生命値が半分以下になったら点滅する
    warning_gauge_material->diffuse_.a_ += warning_flash_speed_ * GameTime::Instance().DeltaTime();
    if (warning_gauge_material->diffuse_.a_ >= 0.5f)
    {
        warning_gauge_material->diffuse_.a_ = 0.5f;
        warning_flash_speed_ *= -1.0f;
    }
    else if (warning_gauge_material->diffuse_.a_ <= 0.0f)
    {
        warning_gauge_material->diffuse_.a_ = 0.0f;
        warning_flash_speed_ *= -1.0f;
    }
}

//--------------------------------------------------------------------------------
//  ライフゲージの更新
//--------------------------------------------------------------------------------
void PlayerUiController::UpdateLifeGauge(const ActorParameter& parameter)
{
    auto life_gauge_material = MainSystem::Instance().GetResources().GetMaterialManager().Get(L"life_gauge");
    if (!life_gauge_material) return;

    // Size
    float life_rate = parameter.GetCurrentLife() / parameter.GetMaxLife();
    Vector3 current_size = kLifeGaugeSize;
    current_size.x_ *= life_rate;
    Vector3& current_center = kLifeGaugeLeftTop + current_size * 0.5f;
    life_gauge_->SetPosition(current_center);
    life_gauge_->SetScale(current_size);

    // Uv
    auto renderer = life_gauge_->GetGameObject().GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    ((MeshRenderer2d*)renderer)->SetUvScale(Vector2(life_rate, 1.0f));

    // 点滅する
    life_gauge_material->diffuse_.r_ += life_flash_speed_ * GameTime::Instance().DeltaTime();
    if (life_gauge_material->diffuse_.r_ >= 0.5f)
    {
        life_gauge_material->diffuse_.r_ = 0.5f;
        life_flash_speed_ *= -1.0f;
    }
    else if (life_gauge_material->diffuse_.r_ <= 0.0f)
    {
        life_gauge_material->diffuse_.r_ = 0.0f;
        life_flash_speed_ *= -1.0f;
    }
    life_gauge_material->diffuse_.g_ = 1.0f;
    life_gauge_material->diffuse_.b_ = life_gauge_material->diffuse_.r_;
}
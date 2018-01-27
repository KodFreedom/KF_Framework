//--------------------------------------------------------------------------------
//  �v���C���[UI�R���g���[��
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_ui_controller.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "main_system.h"
#include "actor_observer.h"
#include "player_controller.h"
#include "resources.h"
#include "material_manager.h"
#include "mesh_renderer_2d.h"
#include "time.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
const Vector3 PlayerUiController::kLifeGaugeSize = Vector3(540.0f, 33.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kLifeGaugeLeftTop = Vector3(-923.0f, 420.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kCoverSize = Vector3(562.0f, 113.0f, 0.0f) * SCREEN_RATE;
const Vector3 PlayerUiController::kCoverLeftTop = Vector3(-935.0f, 400.0f, 0.0f) * SCREEN_RATE;

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
    , warning_gauge_(nullptr)
    , life_gauge_(nullptr)
    , warning_flash_speed_(2.0f)
    , life_flash_speed_(0.2f)
{}

//--------------------------------------------------------------------------------
//  ����������
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
    auto warning_gauge_material = material_manager.Get(L"warning_gauge");
    if (warning_gauge_material)
    {
        warning_gauge_material->diffuse_ = Color(0.5f, 0.2f, 0.2f, 0.0f);
    }
    

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
    auto life_gauge_material = material_manager.Get(L"life_gauge");
    if (life_gauge_material)
    {
        life_gauge_material->diffuse_ = Color::kGreen;
    }
    
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
//  �I������
//--------------------------------------------------------------------------------
void PlayerUiController::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
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
//  ��X�V����
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
//  ���[�j���O�Q�[�W�̍X�V
//--------------------------------------------------------------------------------
void PlayerUiController::UpdateWarningGauge(const ActorParameter& parameter)
{
    auto warning_gauge_material = MainSystem::Instance().GetResources().GetMaterialManager().Get(L"warning_gauge");
    if (!warning_gauge_material) return;

    if (parameter.GetCurrentLife() >= parameter.GetMaxLife() * 0.5f)
    {
        warning_gauge_material->diffuse_.a_ = 0.0f;
        return;
    }

    // �����l�������ȉ��ɂȂ�����_�ł���
    warning_gauge_material->diffuse_.a_ += warning_flash_speed_ * Time::Instance()->DeltaTime();
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
//  ���C�t�Q�[�W�̍X�V
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

    // �_�ł���
    life_gauge_material->diffuse_.r_ += life_flash_speed_ * Time::Instance()->DeltaTime();
    if (life_gauge_material->diffuse_.r_ >= 0.3f)
    {
        life_gauge_material->diffuse_.r_ = 0.3f;
        life_flash_speed_ *= -1.0f;
    }
    else if (life_gauge_material->diffuse_.r_ <= 0.0f)
    {
        life_gauge_material->diffuse_.r_ = 0.0f;
        life_flash_speed_ *= -1.0f;
    }
    life_gauge_material->diffuse_.b_ = life_gauge_material->diffuse_.r_;
}
//--------------------------------------------------------------------------------
//  �G�l�~�[UI�R���g���[��
//  enemy_ui_controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_ui_controller.h"
#include "main_system.h"
#include "actor_observer.h"
#include "game_object.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "transform.h"
#include "camera_manager.h"
#include "camera.h"
#include "game_object_spawner.h"
#include "material_manager.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
const Vector3 EnemyUiController::kLifeGaugeOffset = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 EnemyUiController::kLifeGaugeSize = Vector3(1.5f, 0.25f, 1.0f);
const float EnemyUiController::kSquareDisplayDistance = 50.0f * 50.0f;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructors and destructors
//--------------------------------------------------------------------------------
EnemyUiController::EnemyUiController(GameObject& owner)
    : Behavior(owner, L"EnemyUiController")
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool EnemyUiController::Init(void)
{
    // �}�e���A���̍쐬
    MainSystem::Instance()->GetMaterialManager()
        ->Use(L"enemy_life", Color::kRed, L"polygon.jpg");
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyUiController::Uninit(void)
{
    MainSystem::Instance()->GetMaterialManager()
        ->Disuse(L"enemy_life");
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyUiController::Update(void)
{
    auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
    assert(camera);
    const Vector3& camera_position = camera->GetWorldEyePosition();

    // UI�̐���G�ɍ��킹��
    auto& enemys = MainSystem::Instance()->GetActorObserver()->GetEnemys();
    AdjustUiNumbers(static_cast<int>(enemys.size()) - static_cast<int>(enemy_uis_.size()));

    auto ui_iterator = enemy_uis_.begin();
    for (auto& enemy : enemys)
    {
        const Vector3& enemy_position = enemy->GetGameObject().GetTransform()->GetPosition();

        if (kSquareDisplayDistance >= Vector3::SquareDistanceBetween(camera_position, enemy_position))
        {// �͈͓��Ȃ�X�V����
            (*ui_iterator)->SetActive(true);

            const auto& parameter = enemy->GetParameter();
            float life_rate = parameter.GetCurrentLife() / parameter.GetMaxLife();

            auto transform = (*ui_iterator)->GetTransform();
            transform->SetPosition(enemy_position + kLifeGaugeOffset);
            transform->SetScale(Vector3::Scale(kLifeGaugeSize, Vector3(life_rate, 1.0f, 1.0f)));
        }
        else
        {// ��������Ȃ��ƕ\�����Ȃ�
            (*ui_iterator)->SetActive(false);
        }

        ++ui_iterator;
    }
}

//--------------------------------------------------------------------------------
//  ��X�V����
//--------------------------------------------------------------------------------
void EnemyUiController::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  UI�̐���G�ɍ��킹��
//--------------------------------------------------------------------------------
void EnemyUiController::AdjustUiNumbers(const int difference)
{
    if (difference == 0)
    {
        return;
    }
    else if (difference < 0)
    {// ���炷
        auto iterator = enemy_uis_.begin();

        for (int count = 0; count < -difference; ++count)
        {
            (*iterator)->SetAlive(false);
            iterator = enemy_uis_.erase(iterator);
        }
    }
    else
    {// ���₷
        for (int count = 0; count < difference; ++count)
        {
            enemy_uis_.push_back(GameObjectSpawner::CreateGauge3d(L"enemy_life"));
        }
    }
}
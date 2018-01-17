//--------------------------------------------------------------------------------
//	�_�Ń{�^���R���g���[��
//�@flash_button_controller.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "flash_button_controller.h"
#include "mesh_renderer_2d.h"
#include "game_object.h"
#include "main_system.h"
#include "material_manager.h"
#include "time.h"

//--------------------------------------------------------------------------------
//  
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
FlashButtonController::FlashButtonController(GameObject& owner, const float& flash_speed)
    : Behavior(owner, L"FlashButtonController")
    , flash_speed_(flash_speed)
    , material_(nullptr)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool FlashButtonController::Init(void)
{
    auto renderer = owner_.GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    material_ = MainSystem::Instance()->GetMaterialManager()->GetMaterial(renderer->GetMaterialName());
    return true;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void FlashButtonController::Update(void)
{
    // �_�ł���
    material_->diffuse_.a_ += flash_speed_ * Time::Instance()->DeltaTime();
    if (material_->diffuse_.a_ >= 1.0f)
    {
        material_->diffuse_.a_ = 1.0f;
        flash_speed_ *= -1.0f;
    }
    else if (material_->diffuse_.a_ <= 0.0f)
    {
        material_->diffuse_.a_ = 0.0f;
        flash_speed_ *= -1.0f;
    }
}
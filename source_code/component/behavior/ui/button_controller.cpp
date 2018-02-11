//--------------------------------------------------------------------------------
//  �{�^���R���g���[��
//  button_controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "button_controller.h"
#include "mesh_renderer_2d.h"
#include "game_object.h"
#include "main_system.h"
#include "resources.h"
#include "material_manager.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//  
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ButtonController::ButtonController(GameObject& owner, const String& texture_name)
    : Behavior(owner, L"ButtonController")
{
    auto& material_manager = MainSystem::Instance().GetResources().GetMaterialManager();
    material_manager.Use(texture_name, Color::kWhite, texture_name);
    material_ = material_manager.Get(texture_name);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool ButtonController::Init(void)
{
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ButtonController::Uninit(void)
{
    if (material_)
    {
        MainSystem::Instance().GetResources().GetMaterialManager().Disuse(material_->color_texture_);
        material_ = nullptr;
    }
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ButtonController::Update(void)
{
    if (!material_)
    {
        auto renderer = owner_.GetRendererBy(kMeshRenderer2d);
        assert(renderer);
        material_ = MainSystem::Instance().GetResources().GetMaterialManager().Get(renderer->GetMaterialName());
        return;
    }

    // �F�ύX
    if (time_counter_ < change_time_)
    {
        time_counter_ += GameTime::Instance().DeltaTime();
        material_->diffuse_ = Math::Lerp(material_->diffuse_, target_color_, time_counter_ / change_time_);
    }
}

//--------------------------------------------------------------------------------
//  �F�ύX����
//--------------------------------------------------------------------------------
void ButtonController::ChangeColor(const Color& color, const float& time)
{
    change_time_ = time;
    time_counter_ = 0.0f;
    target_color_ = color;
}
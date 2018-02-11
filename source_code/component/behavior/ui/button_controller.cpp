//--------------------------------------------------------------------------------
//  ボタンコントローラ
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
//  コンストラクタ
//--------------------------------------------------------------------------------
ButtonController::ButtonController(GameObject& owner, const String& texture_name)
    : Behavior(owner, L"ButtonController")
{
    auto& material_manager = MainSystem::Instance().GetResources().GetMaterialManager();
    material_manager.Use(texture_name, Color::kWhite, texture_name);
    material_ = material_manager.Get(texture_name);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ButtonController::Init(void)
{
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
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
//  更新処理
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

    // 色変更
    if (time_counter_ < change_time_)
    {
        time_counter_ += GameTime::Instance().DeltaTime();
        material_->diffuse_ = Math::Lerp(material_->diffuse_, target_color_, time_counter_ / change_time_);
    }
}

//--------------------------------------------------------------------------------
//  色変更処理
//--------------------------------------------------------------------------------
void ButtonController::ChangeColor(const Color& color, const float& time)
{
    change_time_ = time;
    time_counter_ = 0.0f;
    target_color_ = color;
}
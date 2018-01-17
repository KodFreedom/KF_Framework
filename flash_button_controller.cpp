//--------------------------------------------------------------------------------
//	点滅ボタンコントローラ
//　flash_button_controller.cpp
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
//  コンストラクタ
//--------------------------------------------------------------------------------
FlashButtonController::FlashButtonController(GameObject& owner, const float& flash_speed)
    : Behavior(owner, L"FlashButtonController")
    , flash_speed_(flash_speed)
    , material_(nullptr)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool FlashButtonController::Init(void)
{
    auto renderer = owner_.GetRendererBy(kMeshRenderer2d);
    assert(renderer);
    material_ = MainSystem::Instance()->GetMaterialManager()->GetMaterial(renderer->GetMaterialName());
    return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void FlashButtonController::Update(void)
{
    // 点滅する
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
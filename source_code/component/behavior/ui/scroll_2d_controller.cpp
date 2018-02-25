//--------------------------------------------------------------------------------
//  2dスクロールコントローラ
//  scroll_2d_controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "scroll_2d_controller.h"
#include "../../../game_object/game_object.h"
#include "../../renderer/mesh_renderer_2d.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Scroll2dController::Scroll2dController(GameObject& owner, const Short2& pattern, const short frame_per_pattern)
    : Behavior(owner, L"Scroll2dController")
    , pattern_(pattern)
    , frame_per_pattern_(frame_per_pattern)
{
    assert(frame_per_pattern_ > 0);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool Scroll2dController::Init(void)
{
    auto renderer = owner_.GetRendererBy(MeshRendererType::kMeshRenderer2d);
    assert(renderer);
    renderer_ = static_cast<MeshRenderer2d*>(renderer);

    // uv scaleの算出
    Vector2 uv_scale;
    uv_scale.x_ = 1.0f / static_cast<float>(pattern_.x_);
    uv_scale.y_ = 1.0f / static_cast<float>(pattern_.y_);
    renderer_->SetUvScale(uv_scale);

    return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Scroll2dController::Update(void)
{
    assert(renderer_);
    
    // uv更新
    Vector2 offset;
    short real_frame = frame_counter_ / frame_per_pattern_;
    offset.x_ = static_cast<float>(real_frame % pattern_.x_) / static_cast<float>(pattern_.x_);
    offset.y_ = static_cast<float>(real_frame / pattern_.x_) / static_cast<float>(pattern_.y_);
    renderer_->SetUvOffset(offset);

    // frame更新
    frame_counter_ = (frame_counter_ + 1) % (pattern_.x_ * pattern_.y_ * frame_per_pattern_);
}
//--------------------------------------------------------------------------------
//　motion_state.cpp
//  classes for motion state
//  モーションステートのクラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "motion_state.h"
#include "../main_system.h"
#include "../motion_manager.h"
#include "../animator.h"
#include "../motion_data.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//
//  MotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructor / コンストラクタ
//--------------------------------------------------------------------------------
MotionState::MotionState(const String& motion_name, const int start_frame, const MotionStateType& type)
    : current_motion_name_(motion_name), current_frame_counter_(start_frame), type_(type)
{
    current_motion_data_ = MainSystem::Instance()->GetMotionManager()->GetMotionDataBy(current_motion_name_);
}

//--------------------------------------------------------------------------------
//
//  NormalMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モーションの更新
//--------------------------------------------------------------------------------
void NormalMotionState::UpdateMotion(Animator& animator)
{
    assert(current_motion_data_);
    const auto& avatar = animator.GetAvatar();
    assert(avatar.size() <= current_motion_data_->frames_[current_frame_counter_].bone_transforms_.size());
    
    int frame_number = static_cast<int>(current_motion_data_->frames_.size());
    auto iterator = current_motion_data_->frames_[current_frame_counter_].bone_transforms_.begin();

    for (auto& bone : avatar)
    {
        bone.transform->SetPosition(iterator->translation_);
        bone.transform->SetRotation(iterator->rotation_);
        bone.transform->SetScale(iterator->scale_);
        ++iterator;
    }

    if (++current_frame_counter_ >= frame_number
        && current_motion_data_->is_loop_)
    {
        current_frame_counter_ = 0;
    }
}

//--------------------------------------------------------------------------------
//
//  BlendMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructor / コンストラクタ
//--------------------------------------------------------------------------------
BlendMotionState::BlendMotionState(const String& current_motion_name, NormalMotionState* next_motion_pointer, const int current_motion_start_frame, const int blend_frame_number)
    : MotionState(current_motion_name, current_motion_start_frame, kBlendMotionState)
    , next_frame_counter_(0)
    , next_motion_pointer_(next_motion_pointer)
    , blend_frame_counter_(0)
    , blend_frame_number_(blend_frame_number)
{
    next_motion_data_ = MainSystem::Instance()->GetMotionManager()->GetMotionDataBy(next_motion_pointer_->GetCurrentMotionName());
}

//--------------------------------------------------------------------------------
//  モーションの更新
//--------------------------------------------------------------------------------
void BlendMotionState::UpdateMotion(Animator& animator)
{
    assert(current_motion_data_);
    assert(next_motion_data_);
    const auto& avatar = animator.GetAvatar();
    assert(avatar.size() <= current_motion_data_->frames_[current_frame_counter_].bone_transforms_.size());
    assert(avatar.size() <= next_motion_data_->frames_[next_frame_counter_].bone_transforms_.size());

    int frame_number = static_cast<int>(current_motion_data_->frames_.size());
    auto current_iterator = current_motion_data_->frames_[current_frame_counter_].bone_transforms_.begin();

    int next_frame_number = static_cast<int>(next_motion_data_->frames_.size());
    auto next_iterator = next_motion_data_->frames_[next_frame_counter_].bone_transforms_.begin();
    float blend_rate = static_cast<float>(blend_frame_counter_) / static_cast<float>(blend_frame_number_);

    for (auto& bone : avatar)
    {
        const Vector3& blend_translation = Math::Lerp(current_iterator->translation_, next_iterator->translation_, blend_rate);
        const Quaternion& blend_rotation = Math::Slerp(current_iterator->rotation_, next_iterator->rotation_, blend_rate);
        const Vector3& blend_scale = Math::Lerp(current_iterator->scale_, next_iterator->scale_, blend_rate);
        bone.transform->SetPosition(blend_translation);
        bone.transform->SetRotation(blend_rotation);
        bone.transform->SetScale(blend_scale);
        ++current_iterator;
        ++next_iterator;
    }

    if (++current_frame_counter_ == frame_number)
    {
        if (!current_motion_data_->is_loop_) --current_frame_counter_;
        else current_frame_counter_ = 0;
    }

    if (++next_frame_counter_ == next_frame_number)
    {
        if (!next_motion_data_->is_loop_) --next_frame_counter_;
        else next_frame_counter_ = 0;
    }

    ++blend_frame_counter_;
}

//--------------------------------------------------------------------------------
//  モーションの切り替え
//--------------------------------------------------------------------------------
void BlendMotionState::ChangeMotion(Animator& animator)
{
    if (blend_frame_counter_ < blend_frame_number_) return;
    next_motion_pointer_->Set(next_frame_counter_);
    animator.Change(next_motion_pointer_);
}
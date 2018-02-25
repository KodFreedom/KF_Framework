//--------------------------------------------------------------------------------
//  mutant_fall_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_fall_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
#include "mutant_idle_motion_state.h"
void MutantFallMotionState::ChangeMotion(Animator& animator)
{
    if (!current_motion_data_) return;
    if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
    {
        current_frame_counter_ = 0;
    }

    if(animator.GetIsGrounded() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantIdleMotionState(0), current_frame_counter_, 10));
        return;
    }
}
//--------------------------------------------------------------------------------
//  jugg_death_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_death_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
void JuggDeathMotionState::ChangeMotion(Animator& animator)
{
    if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
    {
        --current_frame_counter_;
    }
}
//--------------------------------------------------------------------------------
//  mutant_damaged_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_damaged_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
#include "mutant_idle_motion_state.h"
#include "mutant_dying_motion_state.h"
void MutantDamagedMotionState::ChangeMotion(Animator& animator)
{
    if (current_frame_counter_ >= frame_to_exit_)
    {
        current_frame_counter_ = frame_to_exit_ - 1;
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantIdleMotionState(0), current_frame_counter_, 5));
        return;
    }
    if (animator.GetIsDead() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantDyingMotionState(0), current_frame_counter_, 5));
        return;
    }
}
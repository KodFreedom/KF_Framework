//--------------------------------------------------------------------------------
//  zombie_punching_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "zombie_punching_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
#include "zombie_idle_motion_state.h"
#include "zombie_damaged_motion_state.h"
#include "zombie_death_motion_state.h"
void ZombiePunchingMotionState::ChangeMotion(Animator& animator)
{
    if (current_frame_counter_ >= frame_to_exit_)
    {
        current_frame_counter_ = frame_to_exit_ - 1;
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombieIdleMotionState(0), current_frame_counter_, 10));
        return;
    }
    if(animator.GetIsDamaged() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombieDamagedMotionState(0), current_frame_counter_, 5));
        return;
    }
    if(animator.GetIsDead() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombieDeathMotionState(0), current_frame_counter_, 5));
        return;
    }
}
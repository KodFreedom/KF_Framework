//--------------------------------------------------------------------------------
//  zombie_running_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "zombie_running_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "zombie_walking_motion_state.h"
#include "zombie_damaged_motion_state.h"
#include "zombie_death_motion_state.h"
#include "zombie_kicking_motion_state.h"
#include "zombie_punching_motion_state.h"
void ZombieRunningMotionState::ChangeMotion(Animator& animator)
{
    if (!current_motion_data_) return;
    if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
    {
        current_frame_counter_ = 0;
    }
    if(animator.GetMovement() < 0.500000f)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombieWalkingMotionState(0), current_frame_counter_, 10));
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
    if(animator.GetIsStrongAttack() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombieKickingMotionState(0), current_frame_counter_, 10));
        return;
    }
    if(animator.GetIsLightAttack() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW ZombiePunchingMotionState(0), current_frame_counter_, 10));
        return;
    }
}
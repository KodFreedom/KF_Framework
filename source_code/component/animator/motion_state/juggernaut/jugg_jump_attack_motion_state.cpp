//--------------------------------------------------------------------------------
//  jugg_jump_attack_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_jump_attack_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "jugg_neutral_motion_state.h"
#include "jugg_death_motion_state.h"
void JuggJumpAttackMotionState::ChangeMotion(Animator& animator)
{
    if (current_frame_counter_ >= frame_to_exit_)
    {
        current_frame_counter_ = frame_to_exit_ - 1;
        if (animator.GetIsGrounded() == true
            || animator.GetTimeCounter() >= 3.0f)
        {
            animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggNeutralMotionState(0), current_frame_counter_, 10));
        }
        return;
    }
    if(animator.GetIsDead() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggDeathMotionState(0), current_frame_counter_, 10));
        return;
    }
}
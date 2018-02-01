//--------------------------------------------------------------------------------
//  jugg_flail_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_flail_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "jugg_death_motion_state.h"
#include "jugg_neutral_motion_state.h"
void JuggFlailMotionState::ChangeMotion(Animator& animator)
{
    if(animator.GetIsDead() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggDeathMotionState(0), current_frame_counter_, 10));
        return;
    }
    if(animator.GetIsGrounded() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggNeutralMotionState(0), current_frame_counter_, 10));
        return;
    }
}
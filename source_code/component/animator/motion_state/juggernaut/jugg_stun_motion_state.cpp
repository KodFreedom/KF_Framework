//--------------------------------------------------------------------------------
//  jugg_stun_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_stun_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "jugg_stun_motion_state.h"
#include "jugg_death_motion_state.h"
void JuggStunMotionState::ChangeMotion(Animator& animator)
{
    if(animator.GetIsStun() == false)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggStunMotionState(0), current_frame_counter_, 10));
        return;
    }
    if(animator.GetIsDead() == true)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggDeathMotionState(0), current_frame_counter_, 10));
        return;
    }
}
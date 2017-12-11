//--------------------------------------------------------------------------------
//  jugg_attack_right_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_attack_right_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "jugg_neutral_motion_state.h"
#include "jugg_death_motion_state.h"
void JuggAttackRightMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= frame_to_exit_)
	{
		current_frame_counter_ = frame_to_exit_ - 1;
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggNeutralMotionState(0), current_frame_counter_, 14));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggDeathMotionState(0), current_frame_counter_, 10));
		return;
	}
}
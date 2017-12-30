//--------------------------------------------------------------------------------
//  mutant_jump_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_jump_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "mutant_land_motion_state.h"
void MutantJumpMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
	{
		--current_frame_counter_;
	}
	if(animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantLandMotionState(0), current_frame_counter_, 1));
		return;
	}
}
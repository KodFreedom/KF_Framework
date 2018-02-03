//--------------------------------------------------------------------------------
//  knight_moving_jump_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_moving_jump_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_moving_land_motion_state.h"
#include "knight_death_motion_state.h"
void KnightMovingJumpMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
	{
		--current_frame_counter_;
	}
	if(animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightMovingLandMotionState(0), current_frame_counter_, 1));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightDeathMotionState(0), current_frame_counter_, 5));
		return;
	}
}
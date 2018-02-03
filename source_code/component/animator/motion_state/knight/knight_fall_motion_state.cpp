//--------------------------------------------------------------------------------
//  knight_fall_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_fall_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_hard_land_motion_state.h"
#include "knight_idle_motion_state.h"
void KnightFallMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetTimeCounter() > 1.000000f
	|| animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightHardLandMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightIdleMotionState(0), current_frame_counter_, 5));
		return;
	}
}
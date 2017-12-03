//--------------------------------------------------------------------------------
//  unity_chan_damaged_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "unity_chan_damaged_motion_state.h"
#include "animator.h"
#include "unity_chan_wait_motion_state.h"
void UnityChanDamagedMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= frame_to_exit_)
	{
		current_frame_counter_ = frame_to_exit_ - 1;
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW UnityChanWaitMotionState(0), current_frame_counter_, 5));
		return;
	}
}
//--------------------------------------------------------------------------------
//  unity_chan_lying_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "unity_chan_lying_motion_state.h"
#include "animator.h"
#include "unity_chan_rise_up_motion_state.h"
void UnityChanLyingMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetIsRiseUp() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW UnityChanRiseUpMotionState(0), current_frame_counter_, 5));
		return;
	}
}
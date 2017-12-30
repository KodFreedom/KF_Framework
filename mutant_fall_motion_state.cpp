//--------------------------------------------------------------------------------
//  mutant_fall_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_fall_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "mutant_idle_motion_state.h"
void MutantFallMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantIdleMotionState(0), current_frame_counter_, 10));
		return;
	}
}
//--------------------------------------------------------------------------------
//  knight_block_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_block_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_death_motion_state.h"
#include "knight_end_block_motion_state.h"
#include "knight_block_impact_motion_state.h"
void KnightBlockMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightDeathMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsDefence() == false)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightEndBlockMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDamaged() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightBlockImpactMotionState(0), current_frame_counter_, 5));
		return;
	}
}
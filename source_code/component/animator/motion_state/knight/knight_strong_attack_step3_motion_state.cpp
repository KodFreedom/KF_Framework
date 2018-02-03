//--------------------------------------------------------------------------------
//  knight_strong_attack_step3_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_strong_attack_step3_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_idle_motion_state.h"
#include "knight_death_motion_state.h"
#include "knight_impact_motion_state.h"
void KnightStrongAttackStep3MotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= frame_to_exit_)
	{
		current_frame_counter_ = frame_to_exit_ - 1;
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightIdleMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightDeathMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsDamaged() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightImpactMotionState(0), current_frame_counter_, 5));
		return;
	}
}
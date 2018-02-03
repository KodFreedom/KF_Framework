//--------------------------------------------------------------------------------
//  knight_walk_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_walk_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_impact_motion_state.h"
#include "knight_death_motion_state.h"
#include "knight_run_motion_state.h"
#include "knight_idle_motion_state.h"
#include "knight_moving_jump_motion_state.h"
#include "knight_light_attack_step1_motion_state.h"
#include "knight_strong_attack_step1_motion_state.h"
#include "knight_fall_motion_state.h"
#include "knight_begin_block_motion_state.h"
void KnightWalkMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetIsDamaged() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightImpactMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightDeathMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetMovement() > 0.500000f)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightRunMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetMovement() == 0.000000f)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightIdleMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsJump() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightMovingJumpMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsLightAttack() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightLightAttackStep1MotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsStrongAttack() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightStrongAttackStep1MotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsGrounded() == false)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightFallMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDefence() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightBeginBlockMotionState(0), current_frame_counter_, 5));
		return;
	}
}
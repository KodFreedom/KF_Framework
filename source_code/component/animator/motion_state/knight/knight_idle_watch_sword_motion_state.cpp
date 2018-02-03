//--------------------------------------------------------------------------------
//  knight_idle_watch_sword_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_idle_watch_sword_motion_state.h"
#include "animator.h"
#include "motion_data.h"
#include "knight_idle_motion_state.h"
#include "knight_impact_motion_state.h"
#include "knight_death_motion_state.h"
#include "knight_walk_motion_state.h"
#include "knight_standing_jump_motion_state.h"
#include "knight_light_attack_step1_motion_state.h"
#include "knight_strong_attack_step1_motion_state.h"
#include "knight_all_motion_state.h"
#include "knight_begin_block_motion_state.h"
void KnightIdleWatchSwordMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= frame_to_exit_)
	{
		current_frame_counter_ = frame_to_exit_ - 1;
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightIdleMotionState(0), current_frame_counter_, 10));
		return;
	}
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
	if(animator.GetMovement() > 0.000000f)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightWalkMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsJump() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightStandingJumpMotionState(0), current_frame_counter_, 10));
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
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightAllMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDefence() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW KnightBeginBlockMotionState(0), current_frame_counter_, 5));
		return;
	}
}
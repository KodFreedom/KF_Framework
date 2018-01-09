//--------------------------------------------------------------------------------
//  mutant_idle_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_idle_motion_state.h"
#include "../animator.h"
#include "../motion_data.h"
#include "mutant_walk_motion_state.h"
#include "mutant_jump_motion_state.h"
#include "mutant_punch_motion_state.h"
#include "mutant_swiping_motion_state.h"
#include "mutant_skill_motion_state.h"
#include "mutant_damaged_motion_state.h"
#include "mutant_dying_motion_state.h"
#include "mutant_angry_motion_state.h"
#include "mutant_idle_wake_up_motion_state.h"
#include "mutant_fall_motion_state.h"
void MutantIdleMotionState::ChangeMotion(Animator& animator)
{
	if(animator.GetMovement() > 0.000000f)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantWalkMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsJump() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantJumpMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsLightAttack() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantPunchMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsStrongAttack() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantSwipingMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsSkill() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantSkillMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDamaged() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantDamagedMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantDyingMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsAngry() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantAngryMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetTimeCounter() > 10.000000f)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantIdleWakeUpMotionState(0), current_frame_counter_, 10));
		return;
	}
	if(animator.GetIsGrounded() == false)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantFallMotionState(0), current_frame_counter_, 10));
		return;
	}
}
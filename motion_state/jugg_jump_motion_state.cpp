//--------------------------------------------------------------------------------
//  jugg_jump_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "jugg_jump_motion_state.h"
#include "../animator.h"
#include "../motion_data.h"
#include "jugg_neutral_motion_state.h"
#include "jugg_death_motion_state.h"
#include "jugg_jump_attack_motion_state.h"
void JuggJumpMotionState::ChangeMotion(Animator& animator)
{
	if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
	{
		--current_frame_counter_;
	}
	if(animator.GetIsGrounded() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggNeutralMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsDead() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggDeathMotionState(0), current_frame_counter_, 5));
		return;
	}
	if(animator.GetIsLightAttack() == true)
	{
		animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW JuggJumpAttackMotionState(0), current_frame_counter_, 5));
		return;
	}
}
//--------------------------------------------------------------------------------
//  knight_block_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "knight_block_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
#include "knight_death_motion_state.h"
#include "knight_end_block_motion_state.h"
#include "knight_block_impact_motion_state.h"
void KnightBlockMotionState::ChangeMotion(Animator& animator)
{
    if (!current_motion_data_) return;
    if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
    {
        current_frame_counter_ = 0;
    }
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
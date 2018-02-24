//--------------------------------------------------------------------------------
//  mutant_run_motion_state.cpp
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#include "mutant_run_motion_state.h"
#include "../../animator.h"
#include "../../../../resources/motion_data.h"
#include "mutant_walk_motion_state.h"
#include "mutant_jump_motion_state.h"
#include "mutant_punch_motion_state.h"
#include "mutant_swiping_motion_state.h"
#include "mutant_skill_motion_state.h"
#include "mutant_damaged_motion_state.h"
#include "mutant_dying_motion_state.h"
#include "mutant_angry_motion_state.h"
#include "mutant_fall_motion_state.h"
void MutantRunMotionState::ChangeMotion(Animator& animator)
{
    if (!current_motion_data_) return;
    if (current_frame_counter_ >= static_cast<int>(current_motion_data_->frames_.size()))
    {
        current_frame_counter_ = 0;
    }

    if(animator.GetMovement() < 0.500000f)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantWalkMotionState(0), current_frame_counter_, 5));
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
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantDamagedMotionState(0), current_frame_counter_, 5));
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
    if(animator.GetIsGrounded() == false)
    {
        animator.Change(MY_NEW BlendMotionState(current_motion_name_, MY_NEW MutantFallMotionState(0), current_frame_counter_, 10));
        return;
    }
}
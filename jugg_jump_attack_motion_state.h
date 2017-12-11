//--------------------------------------------------------------------------------
//  jugg_jump_attack_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggJumpAttackMotionState : public NormalMotionState
{
public:
	JuggJumpAttackMotionState(const int start_frame) : NormalMotionState(L"jugg_jump_attack", start_frame) {}
	~JuggJumpAttackMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 37;
};
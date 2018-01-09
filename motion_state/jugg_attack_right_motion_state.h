//--------------------------------------------------------------------------------
//  jugg_attack_right_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggAttackRightMotionState : public NormalMotionState
{
public:
	JuggAttackRightMotionState(const int start_frame) : NormalMotionState(L"jugg_attack_right", start_frame) {}
	~JuggAttackRightMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 45;
};
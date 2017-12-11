//--------------------------------------------------------------------------------
//  jugg_attack_left_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggAttackLeftMotionState : public NormalMotionState
{
public:
	JuggAttackLeftMotionState(const int start_frame) : NormalMotionState(L"jugg_attack_left", start_frame) {}
	~JuggAttackLeftMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 45;
};
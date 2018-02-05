//--------------------------------------------------------------------------------
//  knight_strong_attack_step3_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightStrongAttackStep3MotionState : public NormalMotionState
{
public:
	KnightStrongAttackStep3MotionState(const int start_frame) : NormalMotionState(L"knight_strong_attack_step3", start_frame) {}
	~KnightStrongAttackStep3MotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 131;
};
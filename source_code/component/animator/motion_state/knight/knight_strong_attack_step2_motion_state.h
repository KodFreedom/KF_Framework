//--------------------------------------------------------------------------------
//  knight_strong_attack_step2_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightStrongAttackStep2MotionState : public NormalMotionState
{
public:
	KnightStrongAttackStep2MotionState(const int start_frame) : NormalMotionState(L"knight_strong_attack_step2", start_frame) {}
	~KnightStrongAttackStep2MotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 35;
};
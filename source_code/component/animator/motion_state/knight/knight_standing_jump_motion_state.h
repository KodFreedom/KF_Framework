//--------------------------------------------------------------------------------
//  knight_standing_jump_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightStandingJumpMotionState : public NormalMotionState
{
public:
	KnightStandingJumpMotionState(const int start_frame) : NormalMotionState(L"knight_standing_jump", start_frame) {}
	~KnightStandingJumpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
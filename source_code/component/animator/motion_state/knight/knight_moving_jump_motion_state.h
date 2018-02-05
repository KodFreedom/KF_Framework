//--------------------------------------------------------------------------------
//  knight_moving_jump_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightMovingJumpMotionState : public NormalMotionState
{
public:
	KnightMovingJumpMotionState(const int start_frame) : NormalMotionState(L"knight_moving_jump", start_frame) {}
	~KnightMovingJumpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
//--------------------------------------------------------------------------------
//  unity_chan_jump_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanJumpMotionState : public NormalMotionState
{
public:
	UnityChanJumpMotionState(const int start_frame) : NormalMotionState(L"unity_chan_jump", start_frame) {}
	~UnityChanJumpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
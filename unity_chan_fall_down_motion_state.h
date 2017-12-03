//--------------------------------------------------------------------------------
//  unity_chan_fall_down_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanFallDownMotionState : public NormalMotionState
{
public:
	UnityChanFallDownMotionState(const int start_frame) : NormalMotionState(L"unity_chan_fall_down", start_frame) {}
	~UnityChanFallDownMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 73;
};
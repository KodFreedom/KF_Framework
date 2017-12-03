//--------------------------------------------------------------------------------
//  unity_chan_rise_up_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanRiseUpMotionState : public NormalMotionState
{
public:
	UnityChanRiseUpMotionState(const int start_frame) : NormalMotionState(L"unity_chan_rise_up", start_frame) {}
	~UnityChanRiseUpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 100;
};
//--------------------------------------------------------------------------------
//  unity_chan_wait_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanWaitMotionState : public NormalMotionState
{
public:
	UnityChanWaitMotionState(const int start_frame) : NormalMotionState(L"unity_chan_wait", start_frame) {}
	~UnityChanWaitMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
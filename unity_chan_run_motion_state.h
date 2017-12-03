//--------------------------------------------------------------------------------
//  unity_chan_run_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanRunMotionState : public NormalMotionState
{
public:
	UnityChanRunMotionState(const int start_frame) : NormalMotionState(L"unity_chan_run", start_frame) {}
	~UnityChanRunMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
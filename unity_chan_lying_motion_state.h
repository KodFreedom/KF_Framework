//--------------------------------------------------------------------------------
//  unity_chan_lying_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanLyingMotionState : public NormalMotionState
{
public:
	UnityChanLyingMotionState(const int start_frame) : NormalMotionState(L"unity_chan_lying", start_frame) {}
	~UnityChanLyingMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
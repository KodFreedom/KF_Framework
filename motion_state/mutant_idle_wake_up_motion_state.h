//--------------------------------------------------------------------------------
//  mutant_idle_wake_up_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantIdleWakeUpMotionState : public NormalMotionState
{
public:
	MutantIdleWakeUpMotionState(const int start_frame) : NormalMotionState(L"mutant_idle_wake_up", start_frame) {}
	~MutantIdleWakeUpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 306;
};
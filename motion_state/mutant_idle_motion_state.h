//--------------------------------------------------------------------------------
//  mutant_idle_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantIdleMotionState : public NormalMotionState
{
public:
	MutantIdleMotionState(const int start_frame) : NormalMotionState(L"mutant_idle", start_frame) {}
	~MutantIdleMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
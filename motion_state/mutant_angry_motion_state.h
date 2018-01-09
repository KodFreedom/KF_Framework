//--------------------------------------------------------------------------------
//  mutant_angry_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantAngryMotionState : public NormalMotionState
{
public:
	MutantAngryMotionState(const int start_frame) : NormalMotionState(L"mutant_angry", start_frame) {}
	~MutantAngryMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 268;
};
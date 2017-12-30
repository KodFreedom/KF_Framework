//--------------------------------------------------------------------------------
//  mutant_skill_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantSkillMotionState : public NormalMotionState
{
public:
	MutantSkillMotionState(const int start_frame) : NormalMotionState(L"mutant_skill", start_frame) {}
	~MutantSkillMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 311;
};
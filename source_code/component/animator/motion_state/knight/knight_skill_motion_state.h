//--------------------------------------------------------------------------------
//  knight_skill_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightSkillMotionState : public NormalMotionState
{
public:
	KnightSkillMotionState(const int start_frame) : NormalMotionState(L"knight_skill", start_frame) {}
	~KnightSkillMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 166;
};
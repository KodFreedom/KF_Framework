//--------------------------------------------------------------------------------
//  knight_impact_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightImpactMotionState : public NormalMotionState
{
public:
	KnightImpactMotionState(const int start_frame) : NormalMotionState(L"knight_impact", start_frame) {}
	~KnightImpactMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 36;
};
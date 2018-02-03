//--------------------------------------------------------------------------------
//  knight_block_impact_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightBlockImpactMotionState : public NormalMotionState
{
public:
	KnightBlockImpactMotionState(const int start_frame) : NormalMotionState(L"knight_block_impact", start_frame) {}
	~KnightBlockImpactMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 40;
};
//--------------------------------------------------------------------------------
//  knight_end_block_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightEndBlockMotionState : public NormalMotionState
{
public:
	KnightEndBlockMotionState(const int start_frame) : NormalMotionState(L"knight_end_block", start_frame) {}
	~KnightEndBlockMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 33;
};
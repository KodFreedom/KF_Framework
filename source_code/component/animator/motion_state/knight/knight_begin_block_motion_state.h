//--------------------------------------------------------------------------------
//  knight_begin_block_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightBeginBlockMotionState : public NormalMotionState
{
public:
	KnightBeginBlockMotionState(const int start_frame) : NormalMotionState(L"knight_begin_block", start_frame) {}
	~KnightBeginBlockMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 30;
};
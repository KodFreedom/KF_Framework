//--------------------------------------------------------------------------------
//  knight_block_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightBlockMotionState : public NormalMotionState
{
public:
	KnightBlockMotionState(const int start_frame) : NormalMotionState(L"knight_block", start_frame) {}
	~KnightBlockMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
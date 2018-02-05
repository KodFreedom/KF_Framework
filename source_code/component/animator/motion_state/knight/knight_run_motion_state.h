//--------------------------------------------------------------------------------
//  knight_run_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightRunMotionState : public NormalMotionState
{
public:
	KnightRunMotionState(const int start_frame) : NormalMotionState(L"knight_run", start_frame) {}
	~KnightRunMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
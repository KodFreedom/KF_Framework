//--------------------------------------------------------------------------------
//  knight_idle_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightIdleMotionState : public NormalMotionState
{
public:
	KnightIdleMotionState(const int start_frame) : NormalMotionState(L"knight_idle", start_frame) {}
	~KnightIdleMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
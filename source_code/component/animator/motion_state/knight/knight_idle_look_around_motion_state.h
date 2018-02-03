//--------------------------------------------------------------------------------
//  knight_idle_look_around_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightIdleLookAroundMotionState : public NormalMotionState
{
public:
	KnightIdleLookAroundMotionState(const int start_frame) : NormalMotionState(L"knight_idle_look_around", start_frame) {}
	~KnightIdleLookAroundMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 209;
};
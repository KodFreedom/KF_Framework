//--------------------------------------------------------------------------------
//  knight_standing_land_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightStandingLandMotionState : public NormalMotionState
{
public:
	KnightStandingLandMotionState(const int start_frame) : NormalMotionState(L"knight_standing_land", start_frame) {}
	~KnightStandingLandMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 20;
};
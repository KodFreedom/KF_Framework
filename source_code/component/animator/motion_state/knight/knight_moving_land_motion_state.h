//--------------------------------------------------------------------------------
//  knight_moving_land_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightMovingLandMotionState : public NormalMotionState
{
public:
	KnightMovingLandMotionState(const int start_frame) : NormalMotionState(L"knight_moving_land", start_frame) {}
	~KnightMovingLandMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 15;
};
//--------------------------------------------------------------------------------
//  knight_hard_land_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightHardLandMotionState : public NormalMotionState
{
public:
	KnightHardLandMotionState(const int start_frame) : NormalMotionState(L"knight_hard_land", start_frame) {}
	~KnightHardLandMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 109;
};
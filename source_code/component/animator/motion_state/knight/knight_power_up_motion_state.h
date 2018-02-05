//--------------------------------------------------------------------------------
//  knight_power_up_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightPowerUpMotionState : public NormalMotionState
{
public:
	KnightPowerUpMotionState(const int start_frame) : NormalMotionState(L"knight_power_up", start_frame) {}
	~KnightPowerUpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 136;
};
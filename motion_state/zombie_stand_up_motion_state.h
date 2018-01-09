//--------------------------------------------------------------------------------
//  zombie_stand_up_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieStandUpMotionState : public NormalMotionState
{
public:
	ZombieStandUpMotionState(const int start_frame) : NormalMotionState(L"zombie_stand_up", start_frame) {}
	~ZombieStandUpMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 347;
};
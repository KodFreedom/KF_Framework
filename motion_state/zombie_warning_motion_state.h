//--------------------------------------------------------------------------------
//  zombie_warning_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieWarningMotionState : public NormalMotionState
{
public:
	ZombieWarningMotionState(const int start_frame) : NormalMotionState(L"zombie_warning", start_frame) {}
	~ZombieWarningMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 229;
};
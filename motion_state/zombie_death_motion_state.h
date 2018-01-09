//--------------------------------------------------------------------------------
//  zombie_death_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieDeathMotionState : public NormalMotionState
{
public:
	ZombieDeathMotionState(const int start_frame) : NormalMotionState(L"zombie_death", start_frame) {}
	~ZombieDeathMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
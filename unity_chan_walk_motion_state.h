//--------------------------------------------------------------------------------
//  unity_chan_walk_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class UnityChanWalkMotionState : public NormalMotionState
{
public:
	UnityChanWalkMotionState(const int start_frame) : NormalMotionState(L"unity_chan_walk", start_frame) {}
	~UnityChanWalkMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
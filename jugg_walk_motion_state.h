//--------------------------------------------------------------------------------
//  jugg_walk_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggWalkMotionState : public NormalMotionState
{
public:
	JuggWalkMotionState(const int start_frame) : NormalMotionState(L"jugg_walk", start_frame) {}
	~JuggWalkMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
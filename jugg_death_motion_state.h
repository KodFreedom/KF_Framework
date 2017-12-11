//--------------------------------------------------------------------------------
//  jugg_death_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggDeathMotionState : public NormalMotionState
{
public:
	JuggDeathMotionState(const int start_frame) : NormalMotionState(L"jugg_death", start_frame) {}
	~JuggDeathMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
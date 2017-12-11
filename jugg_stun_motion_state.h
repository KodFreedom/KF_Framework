//--------------------------------------------------------------------------------
//  jugg_stun_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggStunMotionState : public NormalMotionState
{
public:
	JuggStunMotionState(const int start_frame) : NormalMotionState(L"jugg_stun", start_frame) {}
	~JuggStunMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
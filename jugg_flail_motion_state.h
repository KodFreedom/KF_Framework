//--------------------------------------------------------------------------------
//  jugg_flail_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggFlailMotionState : public NormalMotionState
{
public:
	JuggFlailMotionState(const int start_frame) : NormalMotionState(L"jugg_flail", start_frame) {}
	~JuggFlailMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
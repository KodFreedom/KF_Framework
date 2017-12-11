//--------------------------------------------------------------------------------
//  jugg_spain_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggSpainMotionState : public NormalMotionState
{
public:
	JuggSpainMotionState(const int start_frame) : NormalMotionState(L"jugg_spain", start_frame) {}
	~JuggSpainMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
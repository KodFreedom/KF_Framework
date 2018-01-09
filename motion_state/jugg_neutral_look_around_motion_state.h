//--------------------------------------------------------------------------------
//  jugg_neutral_look_around_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggNeutralLookAroundMotionState : public NormalMotionState
{
public:
	JuggNeutralLookAroundMotionState(const int start_frame) : NormalMotionState(L"jugg_neutral_look_around", start_frame) {}
	~JuggNeutralLookAroundMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 187;
};
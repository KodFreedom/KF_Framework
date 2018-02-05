//--------------------------------------------------------------------------------
//  knight_fall_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightFallMotionState : public NormalMotionState
{
public:
	KnightFallMotionState(const int start_frame) : NormalMotionState(L"knight_fall", start_frame) {}
	~KnightFallMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
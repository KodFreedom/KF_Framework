//--------------------------------------------------------------------------------
//  knight_death_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightDeathMotionState : public NormalMotionState
{
public:
	KnightDeathMotionState(const int start_frame) : NormalMotionState(L"knight_death", start_frame) {}
	~KnightDeathMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
//--------------------------------------------------------------------------------
//  knight_walk_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightWalkMotionState : public NormalMotionState
{
public:
	KnightWalkMotionState(const int start_frame) : NormalMotionState(L"knight_walk", start_frame) {}
	~KnightWalkMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
};
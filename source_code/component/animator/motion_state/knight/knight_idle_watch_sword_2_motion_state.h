//--------------------------------------------------------------------------------
//  knight_idle_watch_sword_2_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightIdleWatchSword2MotionState : public NormalMotionState
{
public:
	KnightIdleWatchSword2MotionState(const int start_frame) : NormalMotionState(L"knight_idle_watch_sword_2", start_frame) {}
	~KnightIdleWatchSword2MotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 509;
};
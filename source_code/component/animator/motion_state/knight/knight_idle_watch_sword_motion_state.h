//--------------------------------------------------------------------------------
//  knight_idle_watch_sword_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightIdleWatchSwordMotionState : public NormalMotionState
{
public:
	KnightIdleWatchSwordMotionState(const int start_frame) : NormalMotionState(L"knight_idle_watch_sword", start_frame) {}
	~KnightIdleWatchSwordMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 440;
};
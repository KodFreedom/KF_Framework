//--------------------------------------------------------------------------------
//  knight_magic_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightMagicMotionState : public NormalMotionState
{
public:
	KnightMagicMotionState(const int start_frame) : NormalMotionState(L"knight_magic", start_frame) {}
	~KnightMagicMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 55;
};
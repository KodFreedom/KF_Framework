//--------------------------------------------------------------------------------
//  knight_running_jump_attack_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightRunningJumpAttackMotionState : public NormalMotionState
{
public:
	KnightRunningJumpAttackMotionState(const int start_frame) : NormalMotionState(L"knight_running_jump_attack", start_frame) {}
	~KnightRunningJumpAttackMotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 128;
};
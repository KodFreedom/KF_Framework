//--------------------------------------------------------------------------------
//  knight_light_attack_step1_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class KnightLightAttackStep1MotionState : public NormalMotionState
{
public:
	KnightLightAttackStep1MotionState(const int start_frame) : NormalMotionState(L"knight_light_attack_step1", start_frame) {}
	~KnightLightAttackStep1MotionState() {}

private:
	void ChangeMotion(Animator& animator) override;
	const int frame_to_exit_ = 50;
};
//--------------------------------------------------------------------------------
//  mutant_jump_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantJumpMotionState : public NormalMotionState
{
public:
    MutantJumpMotionState(const int start_frame) : NormalMotionState(L"mutant_jump", start_frame) {}
    ~MutantJumpMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
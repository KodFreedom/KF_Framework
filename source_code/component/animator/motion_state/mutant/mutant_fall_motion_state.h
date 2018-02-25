//--------------------------------------------------------------------------------
//  mutant_fall_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantFallMotionState : public NormalMotionState
{
public:
    MutantFallMotionState(const int start_frame) : NormalMotionState(L"mutant_fall", start_frame) {}
    ~MutantFallMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
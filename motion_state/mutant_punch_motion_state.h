//--------------------------------------------------------------------------------
//  mutant_punch_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantPunchMotionState : public NormalMotionState
{
public:
    MutantPunchMotionState(const int start_frame) : NormalMotionState(L"mutant_punch", start_frame) {}
    ~MutantPunchMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 59;
};
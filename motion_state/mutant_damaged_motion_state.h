//--------------------------------------------------------------------------------
//  mutant_damaged_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantDamagedMotionState : public NormalMotionState
{
public:
    MutantDamagedMotionState(const int start_frame) : NormalMotionState(L"mutant_damaged", start_frame) {}
    ~MutantDamagedMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 93;
};
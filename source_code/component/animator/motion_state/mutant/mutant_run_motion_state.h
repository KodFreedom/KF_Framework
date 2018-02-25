//--------------------------------------------------------------------------------
//  mutant_run_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantRunMotionState : public NormalMotionState
{
public:
    MutantRunMotionState(const int start_frame) : NormalMotionState(L"mutant_run", start_frame) {}
    ~MutantRunMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
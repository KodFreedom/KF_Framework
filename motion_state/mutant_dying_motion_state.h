//--------------------------------------------------------------------------------
//  mutant_dying_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class MutantDyingMotionState : public NormalMotionState
{
public:
    MutantDyingMotionState(const int start_frame) : NormalMotionState(L"mutant_dying", start_frame) {}
    ~MutantDyingMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
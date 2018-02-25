//--------------------------------------------------------------------------------
//  mutant_swiping_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantSwipingMotionState : public NormalMotionState
{
public:
    MutantSwipingMotionState(const int start_frame) : NormalMotionState(L"mutant_swiping", start_frame) {}
    ~MutantSwipingMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 148;
};
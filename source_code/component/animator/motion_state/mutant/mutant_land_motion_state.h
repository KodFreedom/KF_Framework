//--------------------------------------------------------------------------------
//  mutant_land_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantLandMotionState : public NormalMotionState
{
public:
    MutantLandMotionState(const int start_frame) : NormalMotionState(L"mutant_land", start_frame) {}
    ~MutantLandMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 72;
};
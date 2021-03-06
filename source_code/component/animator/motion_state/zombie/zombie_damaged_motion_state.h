//--------------------------------------------------------------------------------
//  zombie_damaged_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class ZombieDamagedMotionState : public NormalMotionState
{
public:
    ZombieDamagedMotionState(const int start_frame) : NormalMotionState(L"zombie_damaged", start_frame) {}
    ~ZombieDamagedMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 100;
};
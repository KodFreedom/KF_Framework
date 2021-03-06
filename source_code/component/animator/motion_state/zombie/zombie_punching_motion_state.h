//--------------------------------------------------------------------------------
//  zombie_punching_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class ZombiePunchingMotionState : public NormalMotionState
{
public:
    ZombiePunchingMotionState(const int start_frame) : NormalMotionState(L"zombie_punching", start_frame) {}
    ~ZombiePunchingMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 218;
};
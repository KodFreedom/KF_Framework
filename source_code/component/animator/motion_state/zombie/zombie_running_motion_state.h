//--------------------------------------------------------------------------------
//  zombie_running_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class ZombieRunningMotionState : public NormalMotionState
{
public:
    ZombieRunningMotionState(const int start_frame) : NormalMotionState(L"zombie_running", start_frame) {}
    ~ZombieRunningMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
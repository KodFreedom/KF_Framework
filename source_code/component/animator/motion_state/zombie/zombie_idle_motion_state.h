//--------------------------------------------------------------------------------
//  zombie_idle_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieIdleMotionState : public NormalMotionState
{
public:
    ZombieIdleMotionState(const int start_frame) : NormalMotionState(L"zombie_idle", start_frame) {}
    ~ZombieIdleMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
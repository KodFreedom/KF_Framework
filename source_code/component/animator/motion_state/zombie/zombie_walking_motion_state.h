//--------------------------------------------------------------------------------
//  zombie_walking_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieWalkingMotionState : public NormalMotionState
{
public:
    ZombieWalkingMotionState(const int start_frame) : NormalMotionState(L"zombie_walking", start_frame) {}
    ~ZombieWalkingMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
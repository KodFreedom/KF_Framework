//--------------------------------------------------------------------------------
//  zombie_scream_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class ZombieScreamMotionState : public NormalMotionState
{
public:
    ZombieScreamMotionState(const int start_frame) : NormalMotionState(L"zombie_scream", start_frame) {}
    ~ZombieScreamMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
    const int frame_to_exit_ = 155;
};
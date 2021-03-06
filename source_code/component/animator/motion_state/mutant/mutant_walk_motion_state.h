//--------------------------------------------------------------------------------
//  mutant_walk_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class MutantWalkMotionState : public NormalMotionState
{
public:
    MutantWalkMotionState(const int start_frame) : NormalMotionState(L"mutant_walk", start_frame) {}
    ~MutantWalkMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
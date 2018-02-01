//--------------------------------------------------------------------------------
//  jugg_neutral_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "motion_state.h"

class JuggNeutralMotionState : public NormalMotionState
{
public:
    JuggNeutralMotionState(const int start_frame) : NormalMotionState(L"jugg_neutral", start_frame) {}
    ~JuggNeutralMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
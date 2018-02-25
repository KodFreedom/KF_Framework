//--------------------------------------------------------------------------------
//  jugg_jump_motion_state.h
//  this is a motion state class which is auto-created by KF_ModelAnalyzer
//--------------------------------------------------------------------------------
#pragma once
#include "../motion_state.h"

class JuggJumpMotionState : public NormalMotionState
{
public:
    JuggJumpMotionState(const int start_frame) : NormalMotionState(L"jugg_jump", start_frame) {}
    ~JuggJumpMotionState() {}

private:
    void ChangeMotion(Animator& animator) override;
};
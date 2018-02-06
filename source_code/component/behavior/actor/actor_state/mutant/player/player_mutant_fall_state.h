//--------------------------------------------------------------------------------
//  mutant�W�����v�X�e�[�g
//  player_mutant_fall_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PlayerMutantFallState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerMutantFallState() : PlayerState(L"PlayerMutantFallState") {}
    ~PlayerMutantFallState() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(PlayerController& actor) override;

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kGroundCheckDistance = 0.1f;
    static constexpr float kMovementMultiplier = 0.25f;
};
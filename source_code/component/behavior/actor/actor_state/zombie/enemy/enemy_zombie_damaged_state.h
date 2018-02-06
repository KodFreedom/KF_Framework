//--------------------------------------------------------------------------------
//  zombie�_���[�W�󂯂��X�e�[�g
//�@enemy_zombie_damaged_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "enemy_state.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class EnemyZombieDamagedState : public EnemyState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    EnemyZombieDamagedState() : EnemyState(L"EnemyZombieDamagedState"), time_counter_(0.0f) {}
    ~EnemyZombieDamagedState() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(EnemyController& enemy) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(EnemyController& enemy) override;

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kWaitTime = 0.5f;

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    float time_counter_;
};
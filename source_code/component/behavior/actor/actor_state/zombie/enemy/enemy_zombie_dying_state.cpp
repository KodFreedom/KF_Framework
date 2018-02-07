//--------------------------------------------------------------------------------
//  zombie���S�X�e�[�g
//�@enemy_zombie_dying_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_dying_state.h"
#include "enemy_controller.h"
#include "animator.h"
#include "game_object.h"
#include "game_time.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetDead(true);
    MainSystem::Instance().GetSoundSystem().Play(kZombieBeatSe);
    MainSystem::Instance().GetSoundSystem().Play(kZombieDeathSe);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Uninit(EnemyController& enemy)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Update(EnemyController& enemy)
{
    time_counter_ += GameTime::Instance().DeltaTime();
    if (time_counter_ >= kWaitTime)
    {
        enemy.GetGameObject().SetAlive(false);
    }
}
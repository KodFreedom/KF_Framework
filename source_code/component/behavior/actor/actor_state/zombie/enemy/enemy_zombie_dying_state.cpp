//--------------------------------------------------------------------------------
//  zombie死亡ステート
//　enemy_zombie_dying_state.cpp
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
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetDead(true);
    MainSystem::Instance().GetSoundSystem().Play(kZombieBeatSe);
    MainSystem::Instance().GetSoundSystem().Play(kZombieDeathSe);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Uninit(EnemyController& enemy)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieDyingState::Update(EnemyController& enemy)
{
    time_counter_ += GameTime::Instance().DeltaTime();
    if (time_counter_ >= kWaitTime)
    {
        enemy.GetGameObject().SetAlive(false);
    }
}
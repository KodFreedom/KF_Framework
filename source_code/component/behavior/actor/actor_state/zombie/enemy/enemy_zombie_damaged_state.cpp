//--------------------------------------------------------------------------------
//  zombieダメージ受けたステート
//　enemy_zombie_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
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
void EnemyZombieDamagedState::Init(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    animator.SetMovement(0.0f);
    animator.SetDamaged(true);
    enemy.SetMovement(Vector3::kZero);
    MainSystem::Instance().GetSoundSystem().Play(kZombieBeatSe);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Update(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    animator.SetDamaged(false);
    time_counter_ += GameTime::Instance().ScaledDeltaTime();
    if (time_counter_ < kInvincibleTime) return;
    
    if (animator.GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (animator.GetCurrentAnimationName()._Equal(L"zombie_idle"))
        {
            if (enemy.GetTarget() != nullptr)
            {// ターゲットに追跡
                enemy.Change(MY_NEW EnemyZombieFollowState);
                return;
            }

            enemy.Change(MY_NEW EnemyZombieWalkState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::OnDamaged(EnemyController& enemy, const float& damage)
{
    if (time_counter_ <= kInvincibleTime) return;

    enemy.ReceiveDamage(damage);

    if (enemy.GetParameter().GetCurrentLife() <= 0.0f)
    {
        enemy.Change(MY_NEW EnemyZombieDyingState);
        return;
    }

    enemy.Change(MY_NEW EnemyZombieDamagedState);
}
//--------------------------------------------------------------------------------
//  zombieダメージ受けたステート
//　enemy_zombie_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Init(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    animator.SetMovement(0.0f);
    animator.SetDamaged(true);
    enemy.SetMovement(Vector3::kZero);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Update(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    if (animator.GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (animator.GetCurrentAnimationName()._Equal(L"zombie_idle"))
        {
            animator.SetDamaged(false);

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
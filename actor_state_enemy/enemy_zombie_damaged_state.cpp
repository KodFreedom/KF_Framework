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
    enemy.GetAnimator().SetDamaged(true);
    enemy.SetMovement(Vector3::kZero);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Uninit(EnemyController& enemy)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Update(EnemyController& enemy)
{
    if (enemy.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!enemy.GetAnimator().GetCurrentAnimationName()._Equal(L"zombie_damaged"))
        {
            enemy.GetAnimator().SetDamaged(false);

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
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

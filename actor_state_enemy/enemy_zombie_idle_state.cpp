//--------------------------------------------------------------------------------
//  zombie待機ステート
//　enemy_zombie_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"
#include "../time.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetGrounded(true);
    enemy.SetMovement(Vector3::kZero);

    auto& parameter = enemy.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(0.0f);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::Uninit(EnemyController& enemy)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::Update(EnemyController& enemy)
{
    time_counter_ += Time::Instance()->DeltaTime();
    enemy.CheckGrounded();
    enemy.Move();

    if (time_counter_ >= kWaitTime)
    {// 次の目的地をランダムで設定
        const Vector3& born_position = enemy.GetBornPosition();
        const float& patrol_range = enemy.GetPatrolRange();
        Vector3& next_position = Random::Range(born_position - Vector3(patrol_range), born_position + Vector3(patrol_range));
        enemy.SetNextPosition(next_position);
        enemy.Change(MY_NEW EnemyZombieWalkState);
        return;
    }

    if (enemy.GetTarget() != nullptr)
    {// ターゲットに追跡
        enemy.Change(MY_NEW EnemyZombieFollowState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Detector"))
    {
        if (other.GetGameObject().GetTag()._Equal(L"Player"))
        {// ターゲット発見
            enemy.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

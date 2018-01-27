//--------------------------------------------------------------------------------
//  zombie歩くステート
//　enemy_zombie_walk_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "enemy_zombie_dying_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"
#ifdef _DEBUG
#include "../main_system.h"
#include "../debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieWalkState::Init(EnemyController& enemy)
{
    auto& parameter = enemy.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieWalkState::Update(EnemyController& enemy)
{
    // 次の位置に移動する
    const Vector3& my_position = enemy.GetGameObject().GetTransform()->GetPosition();
    Vector3& me_to_next = enemy.GetNextPosition() - my_position;
    me_to_next.y_ = 0.0f;
    float square_distance = me_to_next.SquareMagnitude();
    
    if (square_distance <= kArriveDistance * kArriveDistance)
    {// 目標位置にたどり着いた
        enemy.Change(MY_NEW EnemyZombieIdleState);
        return;
    }

    me_to_next /= sqrtf(square_distance);
    enemy.SetMovement(me_to_next);
    enemy.CheckGrounded();
    enemy.Move();

    if (enemy.GetTarget() != nullptr)
    {// ターゲットに追跡
        enemy.Change(MY_NEW EnemyZombieFollowState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieWalkState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Detector"))
    {
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Player"))
        {// ターゲット発見
#ifdef _DEBUG
            MainSystem::Instance().GetDebugObserver().Display(
                enemy.GetGameObject().GetName() + L" が" + other.GetGameObject().GetName() + L"を発見した！ ");
#endif
            enemy.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void EnemyZombieWalkState::OnDamaged(EnemyController& enemy, const float& damage)
{
    enemy.ReceiveDamage(damage);

    if (enemy.GetParameter().GetCurrentLife() <= 0.0f)
    {
        enemy.Change(MY_NEW EnemyZombieDyingState);
        return;
    }

    enemy.Change(MY_NEW EnemyZombieDamagedState);
}
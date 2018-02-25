//--------------------------------------------------------------------------------
//  zombie待機ステート
//　enemy_zombie_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "enemy_zombie_dying_state.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../main_system.h"
#include "../../../enemy_controller.h"
#include "../../../../../../game_object/game_object.h"
#include "../../../../../../game_time.h"
#include "../../../../../../mode/mode.h"
#ifdef _DEBUG
#include "../../../../../../observer/debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::Init(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    animator.SetMovement(0.0f);
    animator.SetGrounded(true);
    enemy.SetMovement(Vector3::kZero);

    auto& parameter = enemy.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(0.0f);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::Update(EnemyController& enemy)
{
    time_counter_ += GameTime::Instance().ScaledDeltaTime();

    //if (time_counter_ >= kWaitTime
    //    && !MainSystem::Instance().GetCurrentMode().GetName()._Equal(L"DemoPlay"))
    //{// 次の目的地をランダムで設定
    //    const Vector3& born_position = enemy.GetBornPosition();
    //    const float& patrol_range = enemy.GetPatrolRange();
    //    Vector3& next_position = Random::Range(born_position - Vector3(patrol_range), born_position + Vector3(patrol_range));
    //    enemy.SetNextPosition(next_position);
    //    enemy.Change(MY_NEW EnemyZombieWalkState);
    //    return;
    //}

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
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Player"))
        {// ターゲット発見
            auto& target = other.GetGameObject().GetTransform()->GetTopParent()->GetGameObject();
            enemy.SetTarget(&target);
#ifdef _DEBUG
            MainSystem::Instance().GetDebugObserver().Display(
                enemy.GetGameObject().GetName() + L" が" + target.GetName() + L"を発見した！ ");
#endif
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void EnemyZombieIdleState::OnDamaged(EnemyController& enemy, const float& damage)
{
    enemy.ReceiveDamage(damage);

    if (enemy.GetParameter().GetCurrentLife() <= 0.0f)
    {
        enemy.Change(MY_NEW EnemyZombieDyingState);
        return;
    }

    enemy.Change(MY_NEW EnemyZombieDamagedState);
}
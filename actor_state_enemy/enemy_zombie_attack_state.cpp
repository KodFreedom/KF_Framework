//--------------------------------------------------------------------------------
//  zombie追跡ステート
//　enemy_zombie_follow_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_attack_state.h"
#include "enemy_zombie_follow_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../game_object.h"
#include "../time.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetLightAttack(true);
    enemy.SetMovement(Vector3::kZero);

    // 攻撃用ColliderをAwakeにする
    auto right_hand = enemy.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
    if (right_hand)
    {
        auto& colliders = right_hand->GetGameObject().GetColliders();
        for (auto& collider : colliders)
        {
            collider->SetTag(L"Weapon");
            collider->SetMode(ColliderMode::kDynamic);
            collider->Awake();
        }
    }
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Uninit(EnemyController& enemy)
{
    // 攻撃用ColliderをSleepにする
    auto right_hand = enemy.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
    if (right_hand)
    {
        auto& colliders = right_hand->GetGameObject().GetColliders();
        for (auto& collider : colliders)
        {
            collider->Sleep();
        }
    }
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Update(EnemyController& enemy)
{
    if (enemy.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (enemy.GetAnimator().GetCurrentAnimationName()._Equal(L"zombie_punching"))
        {
            enemy.GetAnimator().SetLightAttack(false);
            return;
        }

        time_counter_ += Time::Instance()->DeltaTime();

        if (time_counter_ >= kWaitTime)
        {// ターゲットに追跡
            enemy.Change(MY_NEW EnemyZombieFollowState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Body"))
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
void EnemyZombieAttackState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}
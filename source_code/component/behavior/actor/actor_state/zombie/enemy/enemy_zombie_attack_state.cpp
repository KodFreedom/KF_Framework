//--------------------------------------------------------------------------------
//  zombie追跡ステート
//　enemy_zombie_follow_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_attack_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "enemy_zombie_dying_state.h"
#include "enemy_controller.h"
#include "animator.h"
#include "game_object.h"
#include "game_time.h"
#include "player_controller.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Init(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    animator.SetLightAttack(true);
    animator.SetMovement(0.0f);
    enemy.SetMovement(Vector3::kZero);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Update(EnemyController& enemy)
{
    auto& animator = enemy.GetAnimator();
    if (animator.GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (animator.GetCurrentAnimationName()._Equal(L"zombie_punching"))
        {
            int current_frame = animator.GetCurrentFrame();
            if (current_frame == kBeginAttackFrame)
            {
                // 攻撃用ColliderをAwakeする
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
            else if (current_frame == kEndAttackFrame)
            {
                // 攻撃用ColliderをSleepする
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

            animator.SetLightAttack(false);
            return;
        }

        time_counter_ += GameTime::Instance().DeltaTime();

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
    if (self.GetTag()._Equal(L"Weapon"))
    {
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Player"))
        {// ターゲットに当たった, player controllerを取得
            auto& player = other.GetGameObject().GetTransform()->GetTopParent()->GetGameObject();
            auto player_controller = player.GetBehaviorBy(L"PlayerController");
            if (player_controller)
            {
                static_cast<PlayerController*>(player_controller)->Hit(enemy.GetParameter().GetAttack());
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::OnDamaged(EnemyController& enemy, const float& damage)
{
    enemy.ReceiveDamage(damage);

    if (enemy.GetParameter().GetCurrentLife() <= 0.0f)
    {
        enemy.Change(MY_NEW EnemyZombieDyingState);
        return;
    }

    enemy.Change(MY_NEW EnemyZombieDamagedState);
}
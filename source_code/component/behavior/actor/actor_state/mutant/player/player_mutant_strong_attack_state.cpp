//--------------------------------------------------------------------------------
//  mutant重攻撃ステート
//　player_mutant_strong_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_strong_attack_state.h"
#include "player_mutant_dying_state.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetStrongAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetStrongAttack(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    auto& animator = player.GetAnimator();
    if (animator.GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (animator.GetCurrentAnimationName()._Equal(L"mutant_swiping"))
        {
            int current_frame = animator.GetCurrentFrame();
            if (current_frame == kBeginAttackFrame)
            {
                // 攻撃用ColliderをAwakeする
                auto left_hand = player.GetGameObject().GetTransform()->FindChildBy(L"Mutant:LeftHand");
                if (left_hand)
                {
                    auto& colliders = left_hand->GetGameObject().GetColliders();
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
                auto left_hand = player.GetGameObject().GetTransform()->FindChildBy(L"Mutant:LeftHand");
                if (left_hand)
                {
                    auto& colliders = left_hand->GetGameObject().GetColliders();
                    for (auto& collider : colliders)
                    {
                        collider->Sleep();
                    }
                }
            }
        }
        else
        {
            player.Change(MY_NEW PlayerMutantIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Weapon"))
    {
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Enemy"))
        {// ターゲットに当たった, enemy controllerを取得
            auto enemy_controller = other.GetGameObject().GetBehaviorBy(L"EnemyController");
            if (enemy_controller)
            {
                static_cast<EnemyController*>(enemy_controller)->Hit(player.GetParameter().GetAttack() * 1.5f);
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerMutantStrongAttackState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    player.Change(MY_NEW PlayerMutantDamagedState);
}
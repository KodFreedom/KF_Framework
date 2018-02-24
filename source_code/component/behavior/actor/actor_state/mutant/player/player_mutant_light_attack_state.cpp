//--------------------------------------------------------------------------------
//  mutant軽攻撃ステート
//　player_mutant_light_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_damaged_state.h"
#include "player_mutant_light_attack_state.h"
#include "player_mutant_dying_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../enemy_controller.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);

    // 攻撃用ColliderをAwakeする
    auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"Mutant:RightHand");
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
void PlayerMutantLightAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);

    // 攻撃用ColliderをSleepする
    auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"Mutant:RightHand");
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
void PlayerMutantLightAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_punch"))
        {
            player.Change(MY_NEW PlayerMutantIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Weapon"))
    {
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Enemy"))
        {// ターゲットに当たった, enemy controllerを取得
            auto enemy_controller = other.GetGameObject().GetBehaviorBy(L"EnemyController");
            if (enemy_controller)
            {
                static_cast<EnemyController*>(enemy_controller)->Hit(player.GetParameter().GetAttack());
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerMutantLightAttackState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    player.Change(MY_NEW PlayerMutantDamagedState);
}
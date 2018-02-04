//--------------------------------------------------------------------------------
//  knight軽攻撃ステップ1ステート
//　player_knight_light_attack_step1_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_light_attack_step1_state.h"
#include "player_knight_light_attack_step2_state.h"
#include "player_knight_idle_state.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep1State::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep1State::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep1State::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    auto& animator = player.GetAnimator();
    if (animator.GetCurrentAnimationStateType() == kBlendMotionState)
    {
        animator.SetLightAttack(false);
        return;
    }

    int current_frame = animator.GetCurrentFrame();
    if (current_frame == kBeginAttackFrame)
    {
        // 攻撃用ColliderをAwakeする
        auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
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
        auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
        if (right_hand)
        {
            auto& colliders = right_hand->GetGameObject().GetColliders();
            for (auto& collider : colliders)
            {
                collider->Sleep();
            }
        }
    }

    if (player.IsLightAttack())
    {
        animator.SetLightAttack(true);
    }
    
    if (current_frame >= kBeginStep2Frame && animator.GetIsLightAttack() == true)
    {
        player.Change(MY_NEW PlayerKnightLightAttackStep2State);
        return;
    }
}

//--------------------------------------------------------------------------------
//  モーション終了の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep1State::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_light_attack_step1"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep1State::OnTrigger(PlayerController& player, Collider& self, Collider& other)
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
void PlayerKnightLightAttackStep1State::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        //player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    //player.Change(MY_NEW PlayerMutantDamagedState);
}
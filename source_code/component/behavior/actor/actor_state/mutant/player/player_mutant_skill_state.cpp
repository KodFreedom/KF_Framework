//--------------------------------------------------------------------------------
//  mutantスキルステート
//　player_mutant_skill_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_skill_state.h"
#include "player_mutant_dying_state.h"
#include "../../../../../../game_time.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"

//--------------------------------------------------------------------------------
//  初期化関数
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetSkill(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetSkill(false);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Update(PlayerController& player)
{
    time_counter_ -= GameTime::Instance().ScaledDeltaTime();
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"mutant_skill"))
        {
            player.Change(MY_NEW PlayerMutantIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    // Todo : 攻撃の当たり判定
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::OnDamaged(PlayerController& player, const float& damage)
{
    if (time_counter_ > 0.0f) { return; }
    time_counter_ = kNoDamageTime;
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }
}
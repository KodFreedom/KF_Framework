//--------------------------------------------------------------------------------
//  mutant�X�L���X�e�[�g
//�@player_mutant_skill_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_idle_state.h"
#include "player_mutant_skill_state.h"
#include "player_mutant_dying_state.h"
#include "player_controller.h"
#include "animator.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetSkill(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetSkill(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
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
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantSkillState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    // Todo : �U���̓����蔻��
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
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
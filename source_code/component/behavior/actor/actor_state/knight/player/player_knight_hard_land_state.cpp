//--------------------------------------------------------------------------------
//  knight���n�X�e�[�g
//  player_knight_hard_land_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_idle_state.h"
#include "player_knight_hard_land_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightHardLandState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightHardLandState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightHardLandState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_hard_land"))
        {
            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void PlayerKnightHardLandState::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightHardLandState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_hard_land"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}
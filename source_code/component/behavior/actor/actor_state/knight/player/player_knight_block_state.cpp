//--------------------------------------------------------------------------------
//  knight�u���b�N�X�e�[�g
//  player_knight_block_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_block_state.h"
#include "player_knight_idle_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDefence(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetDefence(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    player.GetAnimator().SetDefence(player.IsDefence());
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_block_impact"))
    {
        player.GetAnimator().SetDamaged(false);
    }

    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_end_block"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnDamaged(PlayerController& player, const float& damage)
{
    player.GetAnimator().SetDamaged(true);
    
    // Se
    auto& sound_system = MainSystem::Instance().GetSoundSystem();
    sound_system.Play(kBlockSe);
    sound_system.Play(kGuardVoiceSe);
}
//--------------------------------------------------------------------------------
//  knight�u���b�N�X�e�[�g
//  player_knight_block_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_block_state.h"
#include "player_knight_idle_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../main_system.h"
#include "../../../../../../sound/sound_system.h"

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

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetAnimator().GetCurrentAnimationName().find(L"block") == String::npos)
        {
            player.Change(MY_NEW PlayerKnightIdleState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightBlockState::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_end_block"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }

    player.GetAnimator().SetDamaged(false);
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
    if (player.GetAnimator().GetIsDamaged()) return;
    player.GetAnimator().SetDamaged(true);
    
    // Se
    auto& sound_system = MainSystem::Instance().GetSoundSystem();
    sound_system.Play(kBlockSe);
    sound_system.Play(kGuardVoiceSe);
}
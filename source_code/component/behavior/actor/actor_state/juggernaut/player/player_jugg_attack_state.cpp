//--------------------------------------------------------------------------------
//  jugg�U���X�e�[�g
//�@player_jugg_attack_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_attack_state.h"
#include "player_jugg_neutral_state.h"
#include "player_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        const auto& current_animation_name = player.GetAnimator().GetCurrentAnimationName();
        if (!current_animation_name._Equal(L"jugg_attack_left")
            && !current_animation_name._Equal(L"jugg_attack_right"))
        {
            player.Change(MY_NEW PlayerJuggNeutralState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggAttackState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
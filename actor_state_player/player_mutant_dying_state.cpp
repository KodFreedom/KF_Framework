//--------------------------------------------------------------------------------
//  mutant���S�X�e�[�g
//�@player_mutant_dying_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_dying_state.h"
#include "../player_controller.h"
#include "../animator.h"
#include "../time.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Init(PlayerController& player)
{
    player.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetDead(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    time_counter_ -= Time::Instance()->ScaledDeltaTime();
    if (time_counter_ <= 0.0f)
    {
        player.GetGameObject().SetAlive(false);
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
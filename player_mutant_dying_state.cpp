//--------------------------------------------------------------------------------
//  mutant���S�X�e�[�g
//�@player_mutant_dying_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_mutant_dying_state.h"
#include "actor_controller.h"
#include "animator.h"
#include "time.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Init(ActorController& actor)
{
    actor.GetParameter().SetMovementMultiplier(kMovementMultiplier);
    actor.GetAnimator().SetDead(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Uninit(ActorController& actor)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::Update(ActorController& actor)
{
    PlayerState::Update(actor);
    time_counter_ -= Time::Instance()->ScaledDeltaTime();
    if (time_counter_ <= 0.0f)
    {
        actor.GetGameObject().SetAlive(false);
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerMutantDyingState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}
//--------------------------------------------------------------------------------
//  zombie�ҋ@�X�e�[�g
//�@enemy_zombie_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Init(EnemyController& enemy)
{
    enemy.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    enemy.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Uninit(EnemyController& enemy)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Update(EnemyController& enemy)
{
    enemy.CheckGrounded();
    enemy.Move();
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"detector"))
    {
        if (other.GetGameObject().GetTag()._Equal(L"Player"))
        {
            enemy.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

//--------------------------------------------------------------------------------
//  zombie�_���[�W�󂯂��X�e�[�g
//�@enemy_zombie_damaged_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetDamaged(true);
    enemy.SetMovement(Vector3::kZero);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Uninit(EnemyController& enemy)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::Update(EnemyController& enemy)
{
    if (enemy.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (!enemy.GetAnimator().GetCurrentAnimationName()._Equal(L"zombie_damaged"))
        {
            enemy.GetAnimator().SetDamaged(false);

            if (enemy.GetTarget() != nullptr)
            {// �^�[�Q�b�g�ɒǐ�
                enemy.Change(MY_NEW EnemyZombieFollowState);
                return;
            }

            enemy.Change(MY_NEW EnemyZombieWalkState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieDamagedState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

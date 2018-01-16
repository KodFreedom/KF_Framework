//--------------------------------------------------------------------------------
//  zombie�ǐՃX�e�[�g
//�@enemy_zombie_follow_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_attack_state.h"
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "enemy_zombie_dying_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"
#ifdef _DEBUG
#include "../main_system.h"
#include "../debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::Init(EnemyController& enemy)
{
    enemy.GetParameter().SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::Update(EnemyController& enemy)
{
    // �v���C���[�ɒǐ�
    const Vector3& player_position = enemy.GetTarget()->GetTransform()->GetPosition();
    const Vector3& my_position = enemy.GetGameObject().GetTransform()->GetPosition();
    Vector3& me_to_player = player_position - my_position;
    float square_distance = me_to_player.SquareMagnitude();
    float warning_range = enemy.GetWarningRange() * kWarningRangeMultiplier;
    
    if (square_distance > warning_range * warning_range)
    {// �͈͊O�ɂȂ�̂ŒǐՒ��~
#ifdef _DEBUG
        MainSystem::Instance()->GetDebugObserver()->Display(
            enemy.GetGameObject().GetName() + L" ��" + enemy.GetTarget()->GetName() + L"�����������I ");
#endif
        enemy.SetTarget(nullptr);
        enemy.Change(MY_NEW EnemyZombieWalkState);
        return;
    }

    if (square_distance <= kAttackRange * kAttackRange)
    {// �U���͈͓��ɂȂ�̂ōU������
        enemy.Change(MY_NEW EnemyZombieAttackState);
        return;
    }

    me_to_player = Vector3::Scale(me_to_player, Vector3(1.0f, 0.0f, 1.0f)).Normalized();
    enemy.SetMovement(me_to_player);
    enemy.CheckGrounded();
    enemy.Move();
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::OnDamaged(EnemyController& enemy, const float& damage)
{
    enemy.ReceiveDamage(damage);

    if (enemy.GetParameter().GetCurrentLife() <= 0.0f)
    {
        enemy.Change(MY_NEW EnemyZombieDyingState);
        return;
    }

    enemy.Change(MY_NEW EnemyZombieDamagedState);
}
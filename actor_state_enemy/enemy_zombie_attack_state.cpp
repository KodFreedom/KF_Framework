//--------------------------------------------------------------------------------
//  zombie�ǐՃX�e�[�g
//�@enemy_zombie_follow_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_attack_state.h"
#include "enemy_zombie_follow_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../game_object.h"
#include "../time.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Init(EnemyController& enemy)
{
    enemy.GetAnimator().SetLightAttack(true);
    enemy.SetMovement(Vector3::kZero);

    // �U���pCollider��Awake�ɂ���
    auto right_hand = enemy.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
    if (right_hand)
    {
        auto& colliders = right_hand->GetGameObject().GetColliders();
        for (auto& collider : colliders)
        {
            collider->SetTag(L"Weapon");
            collider->SetMode(ColliderMode::kDynamic);
            collider->Awake();
        }
    }
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Uninit(EnemyController& enemy)
{
    // �U���pCollider��Sleep�ɂ���
    auto right_hand = enemy.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
    if (right_hand)
    {
        auto& colliders = right_hand->GetGameObject().GetColliders();
        for (auto& collider : colliders)
        {
            collider->Sleep();
        }
    }
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::Update(EnemyController& enemy)
{
    if (enemy.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (enemy.GetAnimator().GetCurrentAnimationName()._Equal(L"zombie_punching"))
        {
            enemy.GetAnimator().SetLightAttack(false);
            return;
        }

        time_counter_ += Time::Instance()->DeltaTime();

        if (time_counter_ >= kWaitTime)
        {// �^�[�Q�b�g�ɒǐ�
            enemy.Change(MY_NEW EnemyZombieFollowState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Body"))
    {
        if (other.GetGameObject().GetTag()._Equal(L"Player"))
        {// �^�[�Q�b�g����
            enemy.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyZombieAttackState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}
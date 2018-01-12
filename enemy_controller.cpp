//--------------------------------------------------------------------------------
//  �G�l�~�[�R���g���[��
//�@enemy controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_controller.h"
#include "actor_state_enemy\enemy_state.h"

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
EnemyController::EnemyController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, rigidbody, animator), target_(nullptr), current_state_(nullptr)
{
    Change(MY_NEW NullEnemyState);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool EnemyController::Init(void)
{
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyController::Uninit(void)
{
    if (current_state_)
    {
        current_state_->Uninit(*this);
        MY_DELETE current_state_;
    }
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyController::Update(void)
{
    current_state_->Update(*this);
}

//--------------------------------------------------------------------------------
//  ��X�V����
//--------------------------------------------------------------------------------
void EnemyController::LateUpdate(void)
{
    current_state_->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyController::OnTrigger(Collider& self, Collider& other)
{
    current_state_->OnTrigger(*this, self, other);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void EnemyController::OnCollision(CollisionInfo& info)
{
    current_state_->OnCollision(*this, info);
}

//--------------------------------------------------------------------------------
//  �X�e�[�g�̕ϊ�
//--------------------------------------------------------------------------------
void EnemyController::Change(EnemyState* state)
{
    if (current_state_)
    {
        current_state_->Uninit(*this);
        MY_DELETE current_state_;
    }
    current_state_ = state;
    current_state_->Init(*this);
}

//--------------------------------------------------------------------------------
//  ���̃X�e�[�g�̖��O��Ԃ�
//--------------------------------------------------------------------------------
const String& EnemyController::GetCurrentStateName(void) const
{
    return current_state_->GetName();
}
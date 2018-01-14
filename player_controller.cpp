//--------------------------------------------------------------------------------
//  �v���C���[�R���g���[��
//�@player controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_controller.h"
#include "actor_state_player\player_state.h"
#include "sphere_collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, L"PlayerController", rigidbody, animator), current_state_(nullptr)
{
    Change(MY_NEW NullPlayerState);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool PlayerController::Init(void)
{
    auto collider = MY_NEW SphereCollider(owner_, kDynamic, 1.0f);
    collider->SetOffset(Vector3(0.0f, 1.5f, 0.0f));
    collider->SetTag(L"Body");
    collider->SetTrigger(true);
    owner_.AddCollider(collider);

    ActorController::Init();
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerController::Uninit(void)
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
void PlayerController::Update(void)
{
    current_state_->Update(*this);
}

//--------------------------------------------------------------------------------
//  ��X�V����
//--------------------------------------------------------------------------------
void PlayerController::LateUpdate(void)
{
    current_state_->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerController::OnTrigger(Collider& self, Collider& other)
{
    current_state_->OnTrigger(*this, self, other);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerController::OnCollision(CollisionInfo& info)
{
    current_state_->OnCollision(*this, info);
}

//--------------------------------------------------------------------------------
//  �X�e�[�g�̕ϊ�
//--------------------------------------------------------------------------------
void PlayerController::Change(PlayerState* state)
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
const String& PlayerController::GetCurrentStateName(void) const
{
    return current_state_->GetName();
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void PlayerController::Hit(const float& damage)
{
    ActorController::Hit(damage);
    current_state_->OnDamaged(*this);
}
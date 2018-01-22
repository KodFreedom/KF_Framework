//--------------------------------------------------------------------------------
//  collider�R���|�l���g
//�@collider.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "collider.h"
#include "game_object.h"
#include "main_system.h"
#include "camera_manager.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Collider::Collider(GameObject& owner, const ColliderType& type, const ColliderMode& mode)
    : Component(owner)
    , type_(type)
    , mode_(mode)
    , is_trigger_(false)
    , is_registered_(false)
    , world_(Matrix44::kIdentity)
    , offset_(Matrix44::kIdentity)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool Collider::Init(void)
{
    is_registered_ = true;
    MainSystem::Instance()->GetCollisionSystem()->Register(this);
    world_ = offset_ * owner_.GetTransform()->GetWorldMatrix();
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Collider::Uninit(void)
{
    if (is_registered_)
    {
        MainSystem::Instance()->GetCollisionSystem()->Deregister(this);
    }
    observers_.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
    // Todo : ����͈͓��̓����蔻�肾���������
    //if (MainSystem::Instance()->GetCameraManager()->GetMainCamera()
    //    ->FrustumCulling(owner_.GetTransform()->GetPosition(), CollisionSystem::kMaxCollisionCheckRange))
    //{
    //    Awake();
    //}
    //else
    //{
    //    Sleep();
    //    return;
    //}

    if (mode_ == kDynamic)
    {
        world_ = offset_ * owner_.GetTransform()->GetCurrentWorldMatrix();
    }
}

//--------------------------------------------------------------------------------
//    ���Έʒu�̐ݒ�
//--------------------------------------------------------------------------------
void Collider::SetOffset(const Vector3& position, const Vector3& rotation)
{
    offset_ = Matrix44::RotationYawPitchRoll(rotation);
    offset_.m30_ = position.x_;
    offset_.m31_ = position.y_;
    offset_.m32_ = position.z_;
}

//--------------------------------------------------------------------------------
//  �ꎞ�I�ɓ����蔻�菈�����甲����
//--------------------------------------------------------------------------------
void Collider::Sleep(void)
{
    if (!is_registered_) return;
    is_registered_ = false;
    MainSystem::Instance()->GetCollisionSystem()->Deregister(this);
}

//--------------------------------------------------------------------------------
//  �����蔻�菈���ɓo�^����
//--------------------------------------------------------------------------------
void Collider::Awake(void)
{
    if (is_registered_) return;
    is_registered_ = true;
    MainSystem::Instance()->GetCollisionSystem()->Register(this);
}

//--------------------------------------------------------------------------------
//  mode�̐ݒ�
//--------------------------------------------------------------------------------
void Collider::SetMode(const ColliderMode& mode)
{
    if (mode_ == mode) return;

    if (is_registered_)
    {
        auto collision_system = MainSystem::Instance()->GetCollisionSystem();
        collision_system->Deregister(this);
        mode_ = mode;
        collision_system->Register(this);
    }
    else
    {
        mode_ = mode;
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void Collider::OnTrigger(Collider& other)
{
    for (auto observer : observers_)
    {
        observer->OnTrigger(*this, other);
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void Collider::OnCollision(CollisionInfo& info)
{
    for (auto observer : observers_)
    {
        observer->OnCollision(info);
    }
}

//--------------------------------------------------------------------------------
//  observer�o�^�֐�
//--------------------------------------------------------------------------------
void Collider::Register(Behavior* observer)
{
    assert(observer);
    observers_.push_back(observer);
}

//--------------------------------------------------------------------------------
//  observer�폜�֐�
//--------------------------------------------------------------------------------
void Collider::Deregister(Behavior* observer)
{
    assert(observer);
    observers_.remove(observer);
}
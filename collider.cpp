//--------------------------------------------------------------------------------
//	collider�R���|�l���g
//�@collider.h
//	Author : Xu Wenjie
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
	world_ = offset_ * owner_.GetTransform()->GetCurrentWorldMatrix();
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
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
	if (MainSystem::Instance()->GetCameraManager()->GetMainCamera()
		->IsInRange(owner_.GetTransform()->GetPosition(), CollisionSystem::kMaxCollisionCheckRange))
	{
		Awake();
	}
	else
	{
		Sleep();
		return;
	}

	if (mode_ == kDynamic)
	{
		world_ = offset_ * owner_.GetTransform()->GetCurrentWorldMatrix();
	}
}

//--------------------------------------------------------------------------------
//	���Έʒu�̐ݒ�
//--------------------------------------------------------------------------------
void Collider::SetOffset(const Vector3& position, const Vector3& rotation)
{
	offset_ = Matrix44::RotationYawPitchRoll(rotation);
	offset_.m30_ = position.x_;
	offset_.m31_ = position.y_;
	offset_.m32_ = position.z_;
}

//--------------------------------------------------------------------------------
//	�ꎞ�I�ɓ����蔻�菈�����甲����
//--------------------------------------------------------------------------------
void Collider::Sleep(void)
{
	if (!is_registered_) return;
	is_registered_ = false;
	MainSystem::Instance()->GetCollisionSystem()->Deregister(this);
}

//--------------------------------------------------------------------------------
//	�����蔻�菈���ɓo�^����
//--------------------------------------------------------------------------------
void Collider::Awake(void)
{
	if (is_registered_) return;
	is_registered_ = true;
	MainSystem::Instance()->GetCollisionSystem()->Register(this);
}
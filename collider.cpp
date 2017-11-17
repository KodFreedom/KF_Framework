//--------------------------------------------------------------------------------
//	�R���W�����R���|�l���g
//�@collisionComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "collider.h"
#include "manager.h"
#include "gameObject.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Collider::Collider(GameObject* const owner, const ColliderType& type, const ColliderMode& mode)
	: Component(owner)
	, type(type)
	, mode(mode)
	, isTrigger(false)
	, isRegistered(false)
	, nextWorldMatrix(Matrix44::Identity)
	, offset(Matrix44::Identity)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool Collider::Init(void)
{
	nextWorldMatrix = offset * owner->GetTransform()->GetNextWorldMatrix();
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Collider::Uninit(void)
{
	if (isRegistered)
	{
		CollisionSystem::Instance()->Deregister(this);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
	if (CameraManager::Instance()->GetMainCamera()->IsInRange(
		owner->GetTransform()->GetCurrentPosition(), CollisionSystem::maxCollisionRange))
	{
		Awake();
	}
	else
	{
		Sleep();
		return;
	}

	if (mode == CM_Dynamic)
	{
		nextWorldMatrix = offset * owner->GetTransform()->GetNextWorldMatrix();
	}
}

//--------------------------------------------------------------------------------
//	�֐����FSetOffset
//  �֐������F���Έʒu�̐ݒ�
//	�����F	position�F�ʒu
//			rotation�F��]
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Collider::SetOffset(const Vector3& position, const Vector3& rotation)
{
	offset = Matrix44::RotationYawPitchRoll(rotation);
	offset.Elements[3][0] = position.X;
	offset.Elements[3][1] = position.Y;
	offset.Elements[3][2] = position.Z;
}

//--------------------------------------------------------------------------------
//	�֐����FSleep
//  �֐������F�ꎞ�I�ɓ����蔻�菈�����甲����
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Collider::Sleep(void)
{
	if (!isRegistered) return;
	isRegistered = false;
	CollisionSystem::Instance()->Deregister(this);
}

//--------------------------------------------------------------------------------
//	�֐����FAwake
//  �֐������F�����蔻�菈���ɓo�^����
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Collider::Awake(void)
{
	if (isRegistered) return;
	isRegistered = true;
	CollisionSystem::Instance()->Register(this);
}
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
	nextWorldMatrix = offset * owner->GetTransform()->GetNextWorldMatrix();
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
	isRegistered = true;
	CollisionSystem::Instance()->Register(this);
}
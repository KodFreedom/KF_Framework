//--------------------------------------------------------------------------------
//	�g�����X�t�H�[���R���|�l���g
//�@transform.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "transform.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Transform::Transform(GameObject& owner) : Component(owner)
	, position_(Vector3::kZero)
	, scale_(Vector3::kOne)
	, rotation_(Quaternion::kIdentity)
	, world_(Matrix44::kIdentity)
	, parent_(nullptr)
	, offset_(Matrix44::kIdentity)
{}

//--------------------------------------------------------------------------------
//  �}�g���N�X�Z�o(�e���Ȃ��̏ꍇ�Ăяo�����)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(void)
{
	if (parent_) return;
	CalculateWorldMatrix();
	for (auto& pair : children_) 
	{
		pair.second->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
//  �֐������F�}�g���N�X�Z�o(�e������̏ꍇ�Ăяo�����)
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(const Matrix44& parent)
{
	CalculateWorldMatrix(parent);
	for (auto& pair : children_)
	{
		pair.second->UpdateMatrix(world_);
	}
}

//--------------------------------------------------------------------------------
// �e�o�^����
//--------------------------------------------------------------------------------
void Transform::RegisterParent(Transform* value, const Vector3& offset_translation, const Quaternion& offset_rotation)
{
	if (parent_)
	{//�e������̏ꍇ�O�̐e����폜
		parent_->DeregisterChild(this);
	}
	parent_ = value;
	parent_->RegisterChild(this);
	offset_ = Matrix44::Transform(offset_rotation, offset_translation);
}

//--------------------------------------------------------------------------------
//  �ŐV�̐��E�s��̎擾
//--------------------------------------------------------------------------------
const Matrix44& Transform::GetCurrentWorldMatrix(void) const
{
	auto& world = Matrix44::Transform(rotation_, position_, scale_);
	if (parent_)
	{
		world *= offset_;
		world *= parent_->GetCurrentWorldMatrix();
	}
	return world;
}

//--------------------------------------------------------------------------------
//  �e�ɑ΂��鑊�΍s��̐ݒ�
//--------------------------------------------------------------------------------
void Transform::SetOffset(const Vector3& translation, const Vector3& rotation)
{
	offset_ = Matrix44::Transform(rotation, translation);
}

//--------------------------------------------------------------------------------
//  ������X������]
//--------------------------------------------------------------------------------
void Transform::RotateByPitch(const float& radian)
{
	auto& right = GetRight();
	auto& rotation = Quaternion::RotateAxis(right, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//  ������Y������]
//--------------------------------------------------------------------------------
void Transform::RotateByYaw(const float& radian)
{
	auto& up = GetUp();
	auto& rotation = Quaternion::RotateAxis(up, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//  ������Z������]
//--------------------------------------------------------------------------------
void Transform::RotateByRoll(const float& radian)
{
	auto& forward = GetForward();
	auto& rotation = Quaternion::RotateAxis(forward, radian);
	rotation_ *= rotation;
}

//--------------------------------------------------------------------------------
//	���E���̕����x�N�g���������̎��ɕϊ�����
//--------------------------------------------------------------------------------
Vector3 Transform::TransformDirectionToLocal(const Vector3& direction)
{
	auto& transpose = world_.Transpose();
	return Vector3::TransformNormal(direction, transpose);
}

//--------------------------------------------------------------------------------
//  �q����T��
//--------------------------------------------------------------------------------
Transform* Transform::FindChildBy(const String& name)
{
	if (owner_.GetName()._Equal(name)) return this;
	auto iterator = children_.find(name);
	if (children_.end() == iterator)
	{
		for (auto& pair : children_)
		{
			auto result = pair.second->FindChildBy(name);
			if (result) return result;
		}
		return nullptr;
	}
	return iterator->second;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�s��̎Z�o(�e�Ȃ�)
//--------------------------------------------------------------------------------
void Transform::CalculateWorldMatrix(void)
{
	world_ = Matrix44::Transform(rotation_, position_, scale_);
}

//--------------------------------------------------------------------------------
//	�s��̎Z�o(�e����)
//--------------------------------------------------------------------------------
void Transform::CalculateWorldMatrix(const Matrix44& parent)
{
	CalculateWorldMatrix();
	world_ *= offset_;
	world_ *= parent;
}
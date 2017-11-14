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

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Transform::Transform(GameObject* const owner) : Component(owner)
	, currentPosition(Vector3::Zero)
	, currentScale(Vector3::One)
	, currentForward(Vector3::Forward)
	, currentUp(Vector3::Up)
	, currentRight(Vector3::Right)
	, currentWorldMatrix(Matrix44::Identity)
	, nextPosition(Vector3::Zero)
	, nextScale(Vector3::One)
	, nextForward(Vector3::Forward)
	, nextUp(Vector3::Up)
	, nextRight(Vector3::Right)
	, parent(nullptr)
	, offset(Matrix44::Identity)
{
	children.clear();
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateMatrix
//  �֐������F�}�g���N�X�Z�o(�e���Ȃ��̏ꍇ�Ăяo�����)
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(void)
{
	if (parent) return;
	calculateWorldMatrix();
	for (auto child : children) 
	{
		child->UpdateMatrix(currentWorldMatrix);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateMatrix
//  �֐������F�}�g���N�X�Z�o(�e������̏ꍇ�Ăяo�����)
//	�����F	parent�F�e�̃}�g���N�X
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(const Matrix44& parent)
{
	calculateWorldMatrix(parent);
	for (auto child : children)
	{
		child->UpdateMatrix(currentWorldMatrix);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FSwapParamater
//  �֐������F�p�����[�^�[��������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::SwapParamater(void)
{
	currentPosition = nextPosition;
	currentScale = nextScale;
	currentForward = nextForward;
	currentRight = nextRight;
	currentUp = nextUp;
}

//--------------------------------------------------------------------------------
//	�֐����FRegisterParent
//  �֐������F�e�̒ǉ�
//	�����F	value�F�e�̃|�C���^
//			offsetPosition�F���Έʒu
//			offsetRotation�F���Ή�]
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RegisterParent(Transform* value, const Vector3& offsetPosition, const Vector3& offsetRotation)
{
	if (parent)
	{//�e������̏ꍇ�O�̐e����폜
		parent->DeregisterChild(this);
	}
	parent = value;
	parent->RegisterChild(this);
	offset = Matrix44::Transform(offsetRotation, offsetPosition);
}

//--------------------------------------------------------------------------------
//	�֐����FGetNextWorldMatrix
//  �֐������F���̃t���[���̐��E�s��̎擾
//	�����F	�Ȃ�
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Transform::GetNextWorldMatrix(void) const
{
	auto& nextWorldMatrix = Matrix44::Transform(nextRight, nextUp, nextForward, nextPosition, nextScale);
	if (parent)
	{
		nextWorldMatrix *= offset;
		nextWorldMatrix *= parent->GetNextWorldMatrix();
	}
	return nextWorldMatrix;
}

//--------------------------------------------------------------------------------
//	�֐����FSetNextRotation
//  �֐������F���̉�]�̐ݒ�
//	�����F	value�F��]���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::SetNextRotation(const Quaternion& value)
{
	auto& rotation = value.ToMatrix();
	nextRight.X = rotation.Elements[0][0];
	nextRight.Y = rotation.Elements[0][1];
	nextRight.Z = rotation.Elements[0][2];
	nextUp.X = rotation.Elements[1][0];
	nextUp.Y = rotation.Elements[1][1];
	nextUp.Z = rotation.Elements[1][2];
	nextForward.X = rotation.Elements[2][0];
	nextForward.Y = rotation.Elements[2][1];
	nextForward.Z = rotation.Elements[2][2];
}

//--------------------------------------------------------------------------------
//	�֐����FSetRotNext
//  �֐������F���̉�]�̐ݒ�
//	�����F	value�F��]���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::SetNextRotation(const Vector3& euler)
{
	SetNextRotation(euler.ToQuaternion());
}

//--------------------------------------------------------------------------------
//	�֐����FSetOffset
//  �֐������F�e�ɑ΂��鑊�΍s��̐ݒ�
//	�����F	position�F�ʒu���
//			rotation�F��]���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::SetOffset(const Vector3& position, const Vector3& rotation)
{
	if (!parent) return;
	offset = Matrix44::Transform(rotation, position);
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByUp
//  �֐������F������ɂ���]
//	�����F	up�F�����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByUp(const Vector3& up)
{
	nextUp = up.Normalized();
	nextRight = (nextUp * nextForward).Normalized();
	nextForward = (nextRight * nextUp).Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByForward
//  �֐������F�O�����ɂ���]
//	�����F	forward�F�O����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByForward(const Vector3& forward)
{
	nextForward = forward.Normalized();
	nextUp = (nextForward * nextRight).Normalized();
	nextRight = (nextUp * nextForward).Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByRight
//  �֐������F�E�����ɂ���]
//	�����F	right�F�E����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByRight(const Vector3& right)
{
	nextRight = right.Normalized();
	nextForward = (nextRight * nextUp).Normalized();
	nextUp = (nextForward * nextRight).Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByEuler
//  �֐������F�I�[���[�p����]
//	�����F	euler�F�I�[���[�p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByEuler(const Vector3& euler)
{
	auto& rotation = Matrix44::RotationYawPitchRoll(euler);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByPitch
//  �֐������F������X��(nextRight)����]
//	�����F	radian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByPitch(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextRight, radian);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByYaw
//  �֐������F������Y��(nextUp)����]
//	�����F	radian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByYaw(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextUp, radian);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	�֐����FRotateByRoll
//  �֐������F������Z��(nextForward)����]
//	�����F	radian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::RotateByRoll(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextForward, radian);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	�֐����FTransformDirectionToLocal
//  �֐������F���E���̕����x�N�g���������̎��ɕϊ�����
//	�����F	direction�F�����x�N�g��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
Vector3 Transform::TransformDirectionToLocal(const Vector3& direction)
{
	auto& inverse = currentWorldMatrix.Transpose();
	return Vector3::TransformNormal(direction, inverse);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FcalculateWorldMatrix
//  �֐������F�s��̎Z�o(�e�Ȃ�)
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::calculateWorldMatrix(void)
{
	currentWorldMatrix = Matrix44::Transform(currentRight, currentUp, currentForward, currentPosition, currentScale);
}

//--------------------------------------------------------------------------------
//	�֐����FcalculateWorldMatrix
//  �֐������F�s��̎Z�o(�e����)
//	�����F	mtxParent
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Transform::calculateWorldMatrix(const Matrix44& parent)
{
	calculateWorldMatrix();
	currentWorldMatrix *= offset;
	currentWorldMatrix *= parent;
}
//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@physics_system.cpp
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "physics_system.h"
#include "collision_detector.h"
#include "rigidbody3d.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const Vector3 PhysicsSystem::kGravity = Vector3(0.0f, -9.8f, 0.0f);

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PhysicsSystem::Update(void)
{
	for (auto iterator = collisions_.begin(); iterator != collisions_.end();)
	{
		Resolve(**iterator);
		delete *iterator;
		iterator = collisions_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FClear
//  �֐������F�Փˏ���S���j������
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
	for (auto iterator = collisions_.begin(); iterator != collisions_.end();)
	{
		delete *iterator;
		iterator = collisions_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �Փˏ���
//--------------------------------------------------------------------------------
void PhysicsSystem::Resolve(Collision& collision)
{
	ResolveVelocity(collision);
	ResolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveVelocity(Collision& collision)
{
	//�������x�v�Z
	float separating_velocity = CalculateSeparatingVelocity(collision);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (separating_velocity > 0.0f) return;

	//���˕Ԃ�W���̎Z�o
	float bounciness = collision.rigidbody_one->GetBounciness();
	if (collision.rigidbody_two)
	{
		bounciness += collision.rigidbody_two->GetBounciness();
		bounciness *= 0.5f;
	}

	//���˕Ԃ葬�x�̎Z�o
	float bounciness_velocity = -separating_velocity * bounciness;

	//�Փ˕����ɍ�p�͂��v�Z����
	Vector3 acceleration = collision.rigidbody_one->GetAcceleration();
	if (collision.rigidbody_two)
	{
		acceleration -= collision.rigidbody_two->GetAcceleration();
	}
	float separating_acceleration = acceleration.Dot(collision.normal);
	
	//�Փ˖@���̋t�����ɂȂ��
	if (separating_acceleration < 0.0f)
	{
		bounciness_velocity = max(bounciness_velocity + separating_acceleration * bounciness, 0.0f);
	}

	//���x�����v�Z
	float delta_velocity = bounciness_velocity - separating_velocity;

	//�t���ʎ擾
	float total_inverse_mass = collision.rigidbody_one->GetInverseMass();
	if (collision.rigidbody_two)
	{
		total_inverse_mass += collision.rigidbody_two->GetInverseMass();
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (total_inverse_mass <= 0.0f) return;

	//�Փ˗͌v�Z
	float impulse = delta_velocity / total_inverse_mass;

	//�P�ʋt���ʂ̏Փ˗�
	const Vector3& impulse_per_inverse_mass = collision.normal * impulse;

	//���x�v�Z
	collision.rigidbody_one->AddVelocity(impulse_per_inverse_mass * collision.rigidbody_one->GetInverseMass());
	if (collision.rigidbody_two)
	{
		collision.rigidbody_two->AddVelocity(impulse_per_inverse_mass * -1.0f * collision.rigidbody_two->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  �߂荞�݂̉���
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveInterpenetration(Collision& collision)
{
	//�Փ˂��Ȃ�
	if (collision.penetration <= 0.0f) return;

	//�t���ʌv�Z
	float total_inverse_mass = collision.rigidbody_one->GetInverseMass();
	if (collision.rigidbody_two)
	{
		total_inverse_mass += collision.rigidbody_two->GetInverseMass();
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (total_inverse_mass <= 0.0f) return;

	//���ʒP�ʖ߂�ʌv�Z
	const Vector3& movement_per_inverse_mass = collision.normal * collision.penetration / total_inverse_mass;

	//�eRigidbody�߂�ʒu�v�Z
	collision.rigidbody_one->AddFixedMovement(movement_per_inverse_mass * collision.rigidbody_one->GetInverseMass());
	if (collision.rigidbody_two)
	{
		collision.rigidbody_two->AddFixedMovement(movement_per_inverse_mass * collision.rigidbody_two->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
float PhysicsSystem::CalculateSeparatingVelocity(Collision& collision)
{
	auto relative_velocity = collision.rigidbody_one->GetVelocity();
	if (collision.rigidbody_two) relative_velocity -= collision.rigidbody_two->GetVelocity();
	return relative_velocity.Dot(collision.normal);
}

/*//--------------------------------------------------------------------------------
//  �Փ˖@����X���Ƃ��āA�Փˍ��W�W�̎Z�o
//--------------------------------------------------------------------------------
void PhysicsSystem::calculateCollisionBasis(Collision& collision)
{
	Vector3 vAxisY, vAxisZ;

	//�Փ˖@�������EX���Ɛ��EY���ǂ����Ƃ̊p�x���߂�
	if (fabsf(collision.Normal.X) > fabsf(collision.Normal.Y))
	{//Y
		float fScale = 1.0f / sqrtf(collision.Normal.X * collision.Normal.X
			+ collision.Normal.Z * collision.Normal.Z);

		vAxisZ.X = collision.Normal.Z * fScale;
		vAxisZ.Y = 0.0f;
		vAxisZ.Z = collision.Normal.X * fScale;

		vAxisY.X = collision.Normal.Y * vAxisZ.X;
		vAxisY.Y = collision.Normal.Z * vAxisZ.X
			- collision.Normal.X * vAxisZ.Z;
		vAxisY.Z = -collision.Normal.Y * vAxisZ.X;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.Normal.Y * collision.Normal.Y
			+ collision.Normal.Z * collision.Normal.Z);

		vAxisZ.X = 0.0f;
		vAxisZ.Y = -collision.Normal.Z * fScale;
		vAxisZ.Z = collision.Normal.Y * fScale;

		vAxisY.X = collision.Normal.Y * vAxisZ.Z
			- collision.Normal.Z * vAxisZ.Y;
		vAxisY.Y = -collision.Normal.X * vAxisZ.Z;
		vAxisY.Z = collision.Normal.X * vAxisZ.Y;
	}

	collision.m_mtxToWorld.Elements[0][0] = collision.Normal.X;
	collision.m_mtxToWorld.Elements[0][1] = collision.Normal.Y;
	collision.m_mtxToWorld.Elements[0][2] = collision.Normal.Z;
	collision.m_mtxToWorld.Elements[1][0] = vAxisY.X;
	collision.m_mtxToWorld.Elements[1][1] = vAxisY.Y;
	collision.m_mtxToWorld.Elements[1][2] = vAxisY.Z;
	collision.m_mtxToWorld.Elements[2][0] = vAxisZ.X;
	collision.m_mtxToWorld.Elements[2][1] = vAxisZ.Y;
	collision.m_mtxToWorld.Elements[2][2] = vAxisZ.Z;
}*/
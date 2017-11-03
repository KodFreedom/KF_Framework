//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@KF_PhysicsSystem.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "physicsSystem.h"
#include "collisionDetector.h"
#include "gameObject.h"
#include "transformComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
PhysicsSystem* PhysicsSystem::instance = nullptr;
const Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
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
	for (auto iterator = collisions.begin(); iterator != collisions.end();)
	{
		resolve(**iterator);
		delete *iterator;
		iterator = collisions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FClear
//  �֐������F�Փˏ���S���j������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
	for (auto iterator = collisions.begin(); iterator != collisions.end();)
	{
		delete *iterator;
		iterator = collisions.erase(iterator);
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
void PhysicsSystem::resolve(Collision& collision)
{
	resolveVelocity(collision);
	resolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveVelocity(Collision& collision)
{
	//�������x�v�Z
	float separatingVelocity = calculateSeparatingVelocity(collision);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (separatingVelocity > 0.0f) return;

	//���˕Ԃ�W���̎Z�o
	float bounciness = collision.RigidbodyOne->Bounciness;
	if (collision.RigidbodyTwo)
	{
		bounciness += collision.RigidbodyTwo->Bounciness;
		bounciness *= 0.5f;
	}

	//���˕Ԃ葬�x�̎Z�o
	float bouncinessVelocity = -separatingVelocity * bounciness;

	//�Փ˕����ɍ�p�͂��v�Z����
	auto acceleration = collision.RigidbodyOne->m_vAcceleration;
	if (collision.RigidbodyTwo)
	{
		acceleration -= collision.RigidbodyTwo->m_vAcceleration;
	}
	float separatingAcceleration = acceleration.Dot(collision.Normal);

	//�Փ˖@���̋t�����ɂȂ��
	if (separatingAcceleration < 0.0f)
	{
		bouncinessVelocity += separatingAcceleration * bounciness;
		if (bouncinessVelocity < 0.0f) bouncinessVelocity = 0.0f;
	}

	//���x�����v�Z
	float deltaVelocity = bouncinessVelocity - separatingVelocity;

	//�t���ʎ擾
	float totalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		totalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (totalInverseMass <= 0.0f) return;

	//�Փ˗͌v�Z
	float impulse = deltaVelocity / totalInverseMass;

	//�P�ʋt���ʂ̏Փ˗�
	auto impulsePerInverseMass = collision.Normal * impulse;

	//���x�v�Z
	collision.RigidbodyOne->m_vVelocity += impulsePerInverseMass * collision.RigidbodyOne->m_fInverseMass;;
	if (collision.RigidbodyTwo)
	{
		collision.RigidbodyTwo->m_vVelocity += impulsePerInverseMass * -1.0f * collision.RigidbodyTwo->m_fInverseMass;;
	}
}

//--------------------------------------------------------------------------------
//  �߂荞�݂̉���
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveInterpenetration(Collision& collision)
{
	//�Փ˂��Ȃ�
	if (collision.Penetration <= 0.0f) return;

	//�t���ʌv�Z
	float totalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		totalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (totalInverseMass <= 0.0f) return;

	//���ʒP�ʖ߂�ʌv�Z
	auto movementPerInverseMass = collision.Normal * collision.Penetration / totalInverseMass;

	//�eRigidbody�߂�ʒu�v�Z
	auto transform = collision.RigidbodyOne->GetGameObject()->GetTransformComponent();
	collision.RigidbodyOne->m_vMovement += movementPerInverseMass * collision.RigidbodyOne->m_fInverseMass;

	if (collision.RigidbodyTwo)
	{
		transform = collision.RigidbodyTwo->GetGameObject()->GetTransformComponent();
		collision.RigidbodyTwo->m_vMovement -= movementPerInverseMass * collision.RigidbodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
float PhysicsSystem::calculateSeparatingVelocity(Collision& collision)
{
	auto relativeVelocity = collision.RigidbodyOne->m_vVelocity;
	if (collision.RigidbodyTwo) relativeVelocity -= collision.RigidbodyTwo->m_vVelocity;
	return relativeVelocity.Dot(collision.Normal);
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
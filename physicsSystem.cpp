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
	for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
	{
		resolve(**itr);
		delete *itr;
		itr = m_listCollision.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
	for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
	{
		delete *itr;
		itr = m_listCollision.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �Փˏ��̃��W�X�g��
//--------------------------------------------------------------------------------
void PhysicsSystem::RegisterCollision(Collision* pCollision)
{
	m_listCollision.push_back(pCollision);
}

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PhysicsSystem::uninit(void)
{
	if (!m_listCollision.empty())
	{
		for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
		{
			delete *itr;
			itr = m_listCollision.erase(itr);
		}
	}
}

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
	float fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (fSeparatingVelocity > 0.0f) { return; }

	//���˕Ԃ�W���̎Z�o
	float fBounciness = collision.RigidbodyOne->Bounciness;
	if (collision.RigidbodyTwo)
	{
		fBounciness += collision.RigidbodyTwo->Bounciness;
		fBounciness *= 0.5f;
	}

	//���˕Ԃ葬�x�̎Z�o
	float fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//�Փ˕����ɍ�p�͂��v�Z����
	auto vAccCausedVelocity = collision.RigidbodyOne->m_vAcceleration;
	if (collision.RigidbodyTwo)
	{
		vAccCausedVelocity -= collision.RigidbodyTwo->m_vAcceleration;
	}
	float fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.Normal);

	//�Փ˖@���̋t�����ɂȂ��
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//���x�����v�Z
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//�t���ʎ擾
	float fTotalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		fTotalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0.0f) { return; }

	//�Փ˗͌v�Z
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//�P�ʋt���ʂ̏Փ˗�
	auto vImpulsePerInverseMass = collision.Normal * fImpulse;

	//���x�v�Z
	auto vVelocity = vImpulsePerInverseMass * collision.RigidbodyOne->m_fInverseMass;
	collision.RigidbodyOne->m_vVelocity += vVelocity;

	if (collision.RigidbodyTwo)
	{
		vVelocity = vImpulsePerInverseMass * -1.0f * collision.RigidbodyTwo->m_fInverseMass;
		collision.RigidbodyTwo->m_vVelocity += vVelocity;
	}
}

//--------------------------------------------------------------------------------
//  �߂荞�݂̉���
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveInterpenetration(Collision& collision)
{
	//�Փ˂��Ȃ�
	if (collision.Penetration <= 0.0f) { return; }

	//�t���ʌv�Z
	float fTotalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		fTotalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0.0f) { return; }

	//���ʒP�ʖ߂�ʌv�Z
	auto vMovePerInverseMass = collision.Normal * collision.Penetration / fTotalInverseMass;

	//�e���q�߂�ʒu�v�Z
	auto pTrans = collision.RigidbodyOne->m_pGameObj->GetTransformComponent();
	collision.RigidbodyOne->m_vMovement += vMovePerInverseMass * collision.RigidbodyOne->m_fInverseMass;

	if (collision.RigidbodyTwo)
	{
		pTrans = collision.RigidbodyTwo->m_pGameObj->GetTransformComponent();
		collision.RigidbodyTwo->m_vMovement -= vMovePerInverseMass * collision.RigidbodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
float PhysicsSystem::calculateSeparatingVelocity(Collision& collision)
{
	auto vRelativeVelocity = collision.RigidbodyOne->m_vVelocity;
	if (collision.RigidbodyTwo) { vRelativeVelocity -= collision.RigidbodyTwo->m_vVelocity; }
	return CKFMath::Vec3Dot(vRelativeVelocity, collision.Normal);
}

//--------------------------------------------------------------------------------
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
}

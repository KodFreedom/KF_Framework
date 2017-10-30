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
#include "KF_PhysicsSystem.h"
#include "KF_CollisionUtility.h"
#include "gameObject.h"
#include "transformComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const Vector3 CKFPhysicsSystem::sc_vGravity = Vector3(0.0f, -9.8f, 0.0f);

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
void CKFPhysicsSystem::Update(void)
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
void CKFPhysicsSystem::Clear(void)
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
void CKFPhysicsSystem::RegisterCollision(CCollision* pCollision)
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
void CKFPhysicsSystem::uninit(void)
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
void CKFPhysicsSystem::resolve(CCollision& collision)
{
	resolveVelocity(collision);
	resolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::resolveVelocity(CCollision& collision)
{
	//�������x�v�Z
	float fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (fSeparatingVelocity > 0.0f) { return; }

	//���˕Ԃ�W���̎Z�o
	float fBounciness = collision.m_pRigidBodyOne->Bounciness;
	if (collision.m_pRigidBodyTwo)
	{
		fBounciness += collision.m_pRigidBodyTwo->Bounciness;
		fBounciness *= 0.5f;
	}

	//���˕Ԃ葬�x�̎Z�o
	float fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//�Փ˕����ɍ�p�͂��v�Z����
	auto vAccCausedVelocity = collision.m_pRigidBodyOne->m_vAcceleration;
	if (collision.m_pRigidBodyTwo)
	{
		vAccCausedVelocity -= collision.m_pRigidBodyTwo->m_vAcceleration;
	}
	float fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.m_vCollisionNormal);

	//�Փ˖@���̋t�����ɂȂ��
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//���x�����v�Z
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//�t���ʎ擾
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0.0f) { return; }

	//�Փ˗͌v�Z
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//�P�ʋt���ʂ̏Փ˗�
	auto vImpulsePerInverseMass = collision.m_vCollisionNormal * fImpulse;

	//���x�v�Z
	auto vVelocity = vImpulsePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;
	collision.m_pRigidBodyOne->m_vVelocity += vVelocity;

	if (collision.m_pRigidBodyTwo)
	{
		vVelocity = vImpulsePerInverseMass * -1.0f * collision.m_pRigidBodyTwo->m_fInverseMass;
		collision.m_pRigidBodyTwo->m_vVelocity += vVelocity;
	}
}

//--------------------------------------------------------------------------------
//  �߂荞�݂̉���
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::resolveInterpenetration(CCollision& collision)
{
	//�Փ˂��Ȃ�
	if (collision.m_fPenetration <= 0.0f) { return; }

	//�t���ʌv�Z
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0.0f) { return; }

	//���ʒP�ʖ߂�ʌv�Z
	auto vMovePerInverseMass = collision.m_vCollisionNormal * collision.m_fPenetration / fTotalInverseMass;

	//�e���q�߂�ʒu�v�Z
	auto pTrans = collision.m_pRigidBodyOne->m_pGameObj->GetTransformComponent();
	collision.m_pRigidBodyOne->m_vMovement += vMovePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;

	if (collision.m_pRigidBodyTwo)
	{
		pTrans = collision.m_pRigidBodyTwo->m_pGameObj->GetTransformComponent();
		collision.m_pRigidBodyTwo->m_vMovement -= vMovePerInverseMass * collision.m_pRigidBodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  �������x�̎Z�o
//--------------------------------------------------------------------------------
float CKFPhysicsSystem::calculateSeparatingVelocity(CCollision& collision)
{
	auto vRelativeVelocity = collision.m_pRigidBodyOne->m_vVelocity;
	if (collision.m_pRigidBodyTwo) { vRelativeVelocity -= collision.m_pRigidBodyTwo->m_vVelocity; }
	return CKFMath::Vec3Dot(vRelativeVelocity, collision.m_vCollisionNormal);
}

//--------------------------------------------------------------------------------
//  �Փ˖@����X���Ƃ��āA�Փˍ��W�W�̎Z�o
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::calculateCollisionBasis(CCollision& collision)
{
	Vector3 vAxisY, vAxisZ;

	//�Փ˖@�������EX���Ɛ��EY���ǂ����Ƃ̊p�x���߂�
	if (fabsf(collision.m_vCollisionNormal.X) > fabsf(collision.m_vCollisionNormal.Y))
	{//Y
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.X * collision.m_vCollisionNormal.X
			+ collision.m_vCollisionNormal.Z * collision.m_vCollisionNormal.Z);

		vAxisZ.X = collision.m_vCollisionNormal.Z * fScale;
		vAxisZ.Y = 0.0f;
		vAxisZ.Z = collision.m_vCollisionNormal.X * fScale;

		vAxisY.X = collision.m_vCollisionNormal.Y * vAxisZ.X;
		vAxisY.Y = collision.m_vCollisionNormal.Z * vAxisZ.X
			- collision.m_vCollisionNormal.X * vAxisZ.Z;
		vAxisY.Z = -collision.m_vCollisionNormal.Y * vAxisZ.X;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.Y * collision.m_vCollisionNormal.Y
			+ collision.m_vCollisionNormal.Z * collision.m_vCollisionNormal.Z);

		vAxisZ.X = 0.0f;
		vAxisZ.Y = -collision.m_vCollisionNormal.Z * fScale;
		vAxisZ.Z = collision.m_vCollisionNormal.Y * fScale;

		vAxisY.X = collision.m_vCollisionNormal.Y * vAxisZ.Z
			- collision.m_vCollisionNormal.Z * vAxisZ.Y;
		vAxisY.Y = -collision.m_vCollisionNormal.X * vAxisZ.Z;
		vAxisY.Z = collision.m_vCollisionNormal.X * vAxisZ.Y;
	}

	collision.m_mtxToWorld.Elements[0][0] = collision.m_vCollisionNormal.X;
	collision.m_mtxToWorld.Elements[0][1] = collision.m_vCollisionNormal.Y;
	collision.m_mtxToWorld.Elements[0][2] = collision.m_vCollisionNormal.Z;
	collision.m_mtxToWorld.Elements[1][0] = vAxisY.X;
	collision.m_mtxToWorld.Elements[1][1] = vAxisY.Y;
	collision.m_mtxToWorld.Elements[1][2] = vAxisY.Z;
	collision.m_mtxToWorld.Elements[2][0] = vAxisZ.X;
	collision.m_mtxToWorld.Elements[2][1] = vAxisZ.Y;
	collision.m_mtxToWorld.Elements[2][2] = vAxisZ.Z;
}

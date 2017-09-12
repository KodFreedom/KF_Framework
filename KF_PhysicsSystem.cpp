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
const CKFVec3 CKFPhysicsSystem::sc_vGravity = CKFVec3(0.0f, -9.8f, 0.0f);

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CKFPhysicsSystem::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Uninit(void)
{
	if (!m_listCollision.empty())
	{//������̂���
		assert(!"�R���W�������܂��c���Ă�I");
		for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
		{
			delete *itr;
			itr = m_listCollision.erase(itr);
		}
	}
}

//--------------------------------------------------------------------------------
//  �����[�X����
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Release(void)
{
	Uninit();
	delete this;
}

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
//  �Փˏ��̃��W�X�g��
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::RegisterCollision(CCollision* pCollision)
{
	m_listCollision.push_back(pCollision);
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
	float fBounciness = collision.m_pRigidBodyOne->m_fBounciness;
	if (collision.m_pRigidBodyTwo)
	{
		fBounciness += collision.m_pRigidBodyTwo->m_fBounciness;
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
	if (collision.m_fPenetration <= 0) { return; }

	//�t���ʌv�Z
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0) { return; }

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
	CKFVec3 vAxisY, vAxisZ;

	//�Փ˖@�������EX���Ɛ��EY���ǂ����Ƃ̊p�x���߂�
	if (fabsf(collision.m_vCollisionNormal.m_fX) > fabsf(collision.m_vCollisionNormal.m_fY))
	{//Y
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.m_fX * collision.m_vCollisionNormal.m_fX
			+ collision.m_vCollisionNormal.m_fZ * collision.m_vCollisionNormal.m_fZ);

		vAxisZ.m_fX = collision.m_vCollisionNormal.m_fZ * fScale;
		vAxisZ.m_fY = 0.0f;
		vAxisZ.m_fZ = collision.m_vCollisionNormal.m_fX * fScale;

		vAxisY.m_fX = collision.m_vCollisionNormal.m_fY * vAxisZ.m_fX;
		vAxisY.m_fY = collision.m_vCollisionNormal.m_fZ * vAxisZ.m_fX
			- collision.m_vCollisionNormal.m_fX * vAxisZ.m_fZ;
		vAxisY.m_fZ = -collision.m_vCollisionNormal.m_fY * vAxisZ.m_fX;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.m_fY * collision.m_vCollisionNormal.m_fY
			+ collision.m_vCollisionNormal.m_fZ * collision.m_vCollisionNormal.m_fZ);

		vAxisZ.m_fX = 0.0f;
		vAxisZ.m_fY = -collision.m_vCollisionNormal.m_fZ * fScale;
		vAxisZ.m_fZ = collision.m_vCollisionNormal.m_fY * fScale;

		vAxisY.m_fX = collision.m_vCollisionNormal.m_fY * vAxisZ.m_fZ
			- collision.m_vCollisionNormal.m_fZ * vAxisZ.m_fY;
		vAxisY.m_fY = -collision.m_vCollisionNormal.m_fX * vAxisZ.m_fZ;
		vAxisY.m_fZ = collision.m_vCollisionNormal.m_fX * vAxisZ.m_fY;
	}

	collision.m_mtxToWorld.m_af[0][0] = collision.m_vCollisionNormal.m_fX;
	collision.m_mtxToWorld.m_af[0][1] = collision.m_vCollisionNormal.m_fY;
	collision.m_mtxToWorld.m_af[0][2] = collision.m_vCollisionNormal.m_fZ;
	collision.m_mtxToWorld.m_af[1][0] = vAxisY.m_fX;
	collision.m_mtxToWorld.m_af[1][1] = vAxisY.m_fY;
	collision.m_mtxToWorld.m_af[1][2] = vAxisY.m_fZ;
	collision.m_mtxToWorld.m_af[2][0] = vAxisZ.m_fX;
	collision.m_mtxToWorld.m_af[2][1] = vAxisZ.m_fY;
	collision.m_mtxToWorld.m_af[2][2] = vAxisZ.m_fZ;
}

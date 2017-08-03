//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@KF_PhysicsSystem.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_PhysicsSystem.h"
#include "gameObject.h"
#include "transformComponent.h"
#include "3DRigidbodyComponent.h"

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
		resolve(*itr);
		itr = m_listCollision.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �Փˏ��̃��W�X�g��
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::RegistryCollision(CCollision& collision)
{
	m_listCollision.push_back(collision);
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
	//�ϐ��錾
	//CKFVec3	vAccCausedVelocity;				//�Փ˕����̍�p��(�����x)
	CKFVec3	vImpulsePerInverseMass;			//�P�ʋt���ʂ̏Փ˗�
	CKFVec3	vVelocity;						//�ω��������x
	CKFVec3	vForce;							//���x������̍�p��
	CKFVec3	vPosToCollisionPos;				//���S�_�ƏՓ˓_�̃x�N�g��
	CKFVec3	vWork;							//�v�Z�p�x�N�g��
	CKFVec3	vRotationForce;					//��]��p��
	float	fSeparatingVelocity;			//�������x
	float	fNewSeparatingVelocity;			//�V�����������x
	//float	fAccCausedSeparatingVelocity;	//�Փ˖@����̉����x
	float	fDeltaVelocity;					//���x�̕ω���
	float	fTotalInverseMass;				//���ʂ̑��a
	float	fImpulse;						//�Փ˗�
	//float	fWorldAngle;					//�S�̓I�ɂƂ��Ă̊p�x
	float	fBounciness;					//���˕Ԃ�W��

	//�������x�v�Z
	fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (fSeparatingVelocity > 0.0f) { return; }

	//���˕Ԃ�W���̎Z�o
	fBounciness = collision.m_pRigidBodyOne->m_fBounciness;
	if (collision.m_pRigidBodyTwo)
	{
		fBounciness += collision.m_pRigidBodyTwo->m_fBounciness;
		fBounciness *= 0.5f;
	}

	//���˕Ԃ葬�x�̎Z�o
	fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//�Փ˕����ɍ�p�͂��v�Z����
	//vAccCausedVelocity = collision.m_pRigidBodyOne->m_vAcceleration;
	//if (collision.m_pRigidBodyTwo)
	//{
	//	vAccCausedVelocity -= collision.m_pRigidBodyTwo->m_vAcceleration;
	//}
	//fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.m_vCollisionNormal);

	////�Փ˖@���̋t�����ɂȂ��
	//if (fAccCausedSeparatingVelocity < 0.0f)
	//{
	//	fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
	//	if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	//}

	//���x�����v�Z
	fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//���ʎ擾
	fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0.0f) { return; }

	//�Փ˗͌v�Z
	fImpulse = fDeltaVelocity / fTotalInverseMass;

	//�P�ʋt���ʂ̏Փ˗�
	vImpulsePerInverseMass = collision.m_vCollisionNormal * fImpulse;

	//���x�v�Z
	vVelocity = vImpulsePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;
	collision.m_pRigidBodyOne->m_vVelocity += vVelocity;

	if (collision.m_pRigidBodyOne->m_bRotLock != C3DRigidbodyComponent::AXIS::XYZ)
	{//��]�������S����������Ă��Ȃ��̏ꍇ
		////�����x�͐�
		//vForce = vVelocity * collision.m_pRigidBodyOne->m_fMass;

		////���S�_�ƏՓ˓_�̃x�N�g��
		//vPosToCollisionPos = collision.m_pRigidBodyOne->m_pGameObj->GetTransformComponent()->GetPosNext()
		//	- collision.m_vCollisionPos;

		////�x�N�g�������E���ɂƂ��Ă̊p�x
		//fWorldAngle = atan2f(vPosToCollisionPos.y, vPosToCollisionPos.x);

		////��]��p�͌v�Z
		//vWork = D3DXVECTOR3(vForce.x * cosf(-fWorldAngle) - vForce.y * sinf(-fWorldAngle),
		//	vForce.x * sinf(-fWorldAngle) + vForce.y * cosf(-fWorldAngle), 0.0f);
		//vRotationForce = D3DXVECTOR3(0.0f, vWork.y, 0.0f);
		//vRotationForce = D3DXVECTOR3(vRotationForce.x * cosf(fWorldAngle) - vRotationForce.y * sinf(fWorldAngle),
		//	vRotationForce.x * sinf(fWorldAngle) + vRotationForce.y * cosf(fWorldAngle), 0.0f);

		//g_aPCollision[nNumCollision].apBody[0]->AddForce(vRotationForce, g_aPCollision[nNumCollision].vCollisionPos);
	}

	if (collision.m_pRigidBodyTwo)
	{
		vVelocity = vImpulsePerInverseMass * -1.0f * collision.m_pRigidBodyTwo->m_fInverseMass;
		collision.m_pRigidBodyTwo->m_vVelocity += vVelocity;

		//if (g_aPCollision[nNumCollision].bRotation)
		//{
		//	//�����x�͐�
		//	vForce = g_aPCollision[nNumCollision].apBody[1]->GetMass() * vVelocity / DURATION * ROTATION_DAMPING;

		//	//���S�_�ƏՓ˓_�̃x�N�g��
		//	vPosToCollisionPos = g_aPCollision[nNumCollision].apBody[1]->GetPosCenter() - g_aPCollision[nNumCollision].vCollisionPos;

		//	//�x�N�g�������E���ɂƂ��Ă̊p�x
		//	fWorldAngle = atan2f(vPosToCollisionPos.y, vPosToCollisionPos.x);

		//	//��]��p�͌v�Z
		//	vWork = D3DXVECTOR3(vForce.x * cosf(-fWorldAngle) - vForce.y * sinf(-fWorldAngle),
		//		vForce.x * sinf(-fWorldAngle) + vForce.y * cosf(-fWorldAngle), 0.0f);
		//	vRotationForce = D3DXVECTOR3(0.0f, vWork.y, 0.0f);
		//	vRotationForce = D3DXVECTOR3(vRotationForce.x * cosf(fWorldAngle) - vRotationForce.y * sinf(fWorldAngle),
		//		vRotationForce.x * sinf(fWorldAngle) + vRotationForce.y * cosf(fWorldAngle), 0.0f);

		//	g_aPCollision[nNumCollision].apBody[1]->AddForce(vRotationForce, g_aPCollision[nNumCollision].vCollisionPos);
		//}
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
	CKFVec3 vMovePerInverseMass = collision.m_vCollisionNormal * collision.m_fPenetration / fTotalInverseMass;

	//�e���q�߂�ʒu�v�Z
	CTransformComponent* pTrans = collision.m_pRigidBodyOne->m_pGameObj->GetTransformComponent();
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
	CKFVec3 vRelativeVelocity = collision.m_pRigidBodyOne->m_vVelocity;

	//���q�Ɨ��q�̏Փ�
	if (collision.m_pRigidBodyTwo)
	{
		vRelativeVelocity -= collision.m_pRigidBodyTwo->m_vVelocity;
	}

	//���όv�Z
	float fDot = CKFMath::Vec3Dot(vRelativeVelocity, collision.m_vCollisionNormal);
	return fDot;
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
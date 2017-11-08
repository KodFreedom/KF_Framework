//--------------------------------------------------------------------------------
//
//�@3DRigidbodyComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "physicsSystem.h"
#include "rigidbody3D.h"
#include "collider.h"
#include "sphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Rigidbody3D::Rigidbody3D(GameObject* const owner)
	: Rigidbody(owner, Type::Rigidbody3D)
	, mass(1.0f)
	, inverseMass(1.0f)
	, drag(1.0f)
	, friction(1.0f)
	, bounciness(0.0f)
	, gravityCoefficient(5.0f)
	, movement(Vector3::Zero)
	, velocity(Vector3::Zero)
	, acceleration(Vector3::Zero)
	, forceAccum(Vector3::Zero)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Rigidbody3D::Update(void)
{
	auto transform = owner->GetTransform();

	//�d�͉����x
	acceleration += PhysicsSystem::Gravity * gravityCoefficient;

	//�͂�������x���v�Z����
	acceleration += forceAccum * inverseMass;
	
	//��]�͂����]�����x���v�Z����
	//Matrix44 mtxIitWorld;
	//calculateInertiaTensorWorld(mtxIitWorld);
	//Vector3 vAngularAcceleration = CKFMath::Vec3TransformCoord(m_vTorqueAccum, mtxIitWorld);

	//���x
	velocity += acceleration * DELTA_TIME;
	//m_vAngularVelocity   += vAngularAcceleration;

	//�ʒu�X�V
	transform->SetNextPosition(transform->GetNextPosition() + velocity * DELTA_TIME + movement);

	//��]�X�V
	//pTrans->RotByEuler(m_vAngularVelocity);

	//��������
	forceAccum = Vector3::Zero;
	acceleration = Vector3::Zero;
	//m_vTorqueAccum = Vector3(0.0f);
	velocity *= drag;
	//m_vAngularVelocity *= AngularDrag;
	movement = Vector3::Zero;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Rigidbody3D::LateUpdate(void)
{
	auto transform = owner->GetTransform();
	transform->SetNextPosition(transform->GetNextPosition() + movement);
	movement = Vector3::Zero;
}

//--------------------------------------------------------------------------------
//  ���ʂ̐ݒ�
//--------------------------------------------------------------------------------
void Rigidbody3D::SetMass(const float& value)
{
	assert(0.0f == value);
	mass = value;
	inverseMass = 1.0f / value;
}

//--------------------------------------------------------------------------------
//  �����e���\���̎Z�o
//--------------------------------------------------------------------------------
//void Rigidbody3D::SetInertiaTensor(Collider* pCollider)
//{
//	float fV00 = 1.0f;
//	float fV11 = 1.0f;
//	float fV22 = 1.0f;
//	ColliderType type = pCollider->GetType();
//	if (type == Sphere)
//	{
//		float fRadius = dynamic_cast<SphereCollider*>(pCollider)->GetRadius();
//		fV00 
//			= fV11
//			= fV22
//			= 0.4f * mass * fRadius * fRadius;
//	}
//	else if (type == AABB)
//	{
//		Vector3 vSize = dynamic_cast<AABBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = mass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
//	}
//	else if (type == OBB)
//	{
//		Vector3 vSize = dynamic_cast<OBBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = mass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
//	}
//
//	m_mtxInertisTensor = {
//		fV00, 0.0f, 0.0f, 0.0f,
//		0.0f, fV11, 0.0f, 0.0f,
//		0.0f, 0.0f, fV22, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f };
//}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���[���h�����e���\���̎Z�o
//--------------------------------------------------------------------------------
//void Rigidbody3D::calculateInertiaTensorWorld(Matrix44& mtxIitWorld)
//{
//	auto listCollider = m_pGameObj->GetColliders();
//	if (!listCollider.empty())
//	{
//		//���[���h�}�g���N�X�̎擾
//		auto itrBegin = listCollider.begin();
//		Matrix44 mtxWorld = (*itrBegin)->GetMatrixWorld();
//		
//		//�ړ��ʂ𖳂���
//		mtxWorld.Elements[3][0] = 0.0f;
//		mtxWorld.Elements[3][1] = 0.0f;
//		mtxWorld.Elements[3][2] = 0.0f;
//
//		//���[���h�����e���\���̎Z�o
//		mtxIitWorld = m_mtxInertisTensor * mtxWorld;
//		
//		//�t�s��̎Z�o
//		mtxIitWorld = CKFMath::MtxTranspose(mtxIitWorld);
//	}
//
//	/*auto listCollider = m_pGameObj->GetColliders();
//	if (!listCollider.empty())
//	{
//		auto itrBegin = listCollider.begin();
//		const Matrix44& mtxWorld = (*itrBegin)->GetMatrixWorld();
//		float fT4 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT9 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT14 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT28 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT33 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT38 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT52 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT57 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT62 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[2][2];
//
//		mtxIitWorld.Elements[0][0] = fT4 * mtxWorld.Elements[0][0]
//			+ fT9 * mtxWorld.Elements[1][0]
//			+ fT14 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][0] = fT4 * mtxWorld.Elements[0][1]
//			+ fT9 * mtxWorld.Elements[1][1]
//			+ fT14 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][0] = fT4 * mtxWorld.Elements[0][2]
//			+ fT9 * mtxWorld.Elements[1][2]
//			+ fT14 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][1] = fT28 * mtxWorld.Elements[0][0]
//			+ fT33 * mtxWorld.Elements[1][0]
//			+ fT38 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][1] = fT28 * mtxWorld.Elements[0][1]
//			+ fT33 * mtxWorld.Elements[1][1]
//			+ fT38 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][1] = fT28 * mtxWorld.Elements[0][2]
//			+ fT33 * mtxWorld.Elements[1][2]
//			+ fT38 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][2] = fT52 * mtxWorld.Elements[0][0]
//			+ fT57 * mtxWorld.Elements[1][0]
//			+ fT62 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][2] = fT52 * mtxWorld.Elements[0][1]
//			+ fT57 * mtxWorld.Elements[1][1]
//			+ fT62 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][2] = fT52 * mtxWorld.Elements[0][2]
//			+ fT57 * mtxWorld.Elements[1][2]
//			+ fT62 * mtxWorld.Elements[2][2];
//	}*/
//}
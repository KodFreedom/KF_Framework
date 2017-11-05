//--------------------------------------------------------------------------------
//
//�@3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "rigidbody.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class Rigidbody3D : public Rigidbody
{
	friend class PhysicsSystem;
public:
	Rigidbody3D(GameObject* const owner);
	~Rigidbody3D() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override;

	//Get�֐�
	auto	GetVelocity(void) const { return velocity; }

	//Set�֐�
	void	SetDrag(const float& value) { drag = value; }
	void	SetMass(const float& value);
	void	SetVelocity(const Vector3& value) { velocity = value; }
	void	AddForce(const Vector3& force) { forceAccum += force; }
	void	Move(const Vector3& value) { movement += value; }
	//void	SetInertiaTensor(Collider* pCollider);

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	//enum AXIS
	//{
	//	X = 0x01,
	//	Y = 0x02,
	//	Z = 0x04,
	//	XYZ = 0x07
	//};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	//void		calculateInertiaTensorWorld(Matrix44& mtxIitWorld);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	float		mass;					//����
	float		inverseMass;				//���ʂ̋t��
	float		drag;					//��R�W��(��C��R)
	//float		AngularDrag;				//��]��R�W��
	float		friction;				//���C�W��
	float		bounciness;				//���˕Ԃ�W��
	float		gravityCoefficient;		//�d�͌W��
	Vector3		movement;				//�ړ���
	Vector3		velocity;				//���x
	Vector3		acceleration;			//�����x
	//Vector3	m_vAngularVelocity;			//��]���x
	Vector3		forceAccum;				//���킹����p��
	//Vector3	m_vTorqueAccum;				//��]��
	//Matrix44	m_mtxInertisTensor;			//�����e���\���̍s��
	//BYTE		m_bRotLock;					//��]�����̃t���O
};
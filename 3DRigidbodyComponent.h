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
#include "rigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CKFPhysicsSystem;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D�����R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent : public CRigidbodyComponent
{
	friend CKFPhysicsSystem;

public:
	C3DRigidbodyComponent(CGameObject* const pGameObj);
	~C3DRigidbodyComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override;

	//Get�֐�
	CKFVec3	GetVelocity(void) const { return m_vVelocity; }

	//Set�֐�
	void	SetDrag(const float& fDrag) { m_fDrag = fDrag; }
	void	MovePos(const CKFVec3& vMovement) { m_vMovement += vMovement; }
	void	SetMass(const float& fMass);
	void	SetVelocity(const CKFVec3& vVelocity) { m_vVelocity = vVelocity; }
	void	AddForce(const CKFVec3& vForce) { m_vForceAccum += vForce; }
	//void	SetInertiaTensor(CColliderComponent* pCollider);

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	enum AXIS
	{
		X = 0x01,
		Y = 0x02,
		Z = 0x04,
		XYZ = 0x07
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	//void		calculateInertiaTensorWorld(CKFMtx44& mtxIitWorld);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	float		m_fMass;					//����
	float		m_fInverseMass;				//���ʂ̋t��
	float		m_fDrag;					//��R�W��(��C��R)
	//float		m_fAngularDrag;				//��]��R�W��
	float		m_fFriction;				//���C�W��
	float		m_fBounciness;				//���˕Ԃ�W��
	float		m_fGravityCoefficient;		//�d�͌W��
	CKFVec3		m_vMovement;				//�ړ���
	CKFVec3		m_vVelocity;				//���x
	CKFVec3		m_vAcceleration;			//�����x
	//CKFVec3		m_vAngularVelocity;			//��]���x
	CKFVec3		m_vForceAccum;				//���킹����p��
	//CKFVec3		m_vTorqueAccum;				//��]��
	//CKFMtx44	m_mtxInertisTensor;			//�����e���\���̍s��
	//BYTE		m_bRotLock;					//��]�����̃t���O
};
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

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D�����R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent : public CRigidbodyComponent
{
public:
	C3DRigidbodyComponent(CGameObject* const pGameObj);
	~C3DRigidbodyComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;

	//Get�֐�
	CKFVec3	GetVelocity(void) const { return m_vVelocity; }
	bool	IsOnGround(void) const { return m_bOnGround; }

	//Set�֐�
	void	MovePos(const CKFVec3& vMovement) { m_vMovement = vMovement; }
	void	SetMass(const float& fMass);
	void	SetVelocity(const CKFVec3& vVelocity) { m_vVelocity = vVelocity; }
	void	SetOnGround(const bool& bValue) { m_bOnGround = bValue; }
	void	AddForce(const CKFVec3& vForce) { m_vForceAccum += vForce; }

private:
	float	m_fMass;		//����
	float	m_fInverseMass;	//���ʂ̋t��
	float	m_fDrag;		//��R�W��
	CKFVec3	m_vGravity;		//�d��
	CKFVec3	m_vMovement;	//�ړ���
	CKFVec3	m_vVelocity;	//���x
	CKFVec3	m_vForceAccum;	//���킹����p��
	CKFVec3	m_vAcceleration;//�����x
	bool	m_bOnGround;	//�����t���b�O
};
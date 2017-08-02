//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@KF_PhysicsSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W����
//--------------------------------------------------------------------------------
class CCollision
{
public:
	CCollision()
		: m_pRigidBodyOne(nullptr)
		, m_pRigidBodyTwo(nullptr)
		, m_vCollisionNormal(CKFVec3(0.0f))
		, m_vCollisionPos(CKFVec3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollision() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	CKFVec3					m_vCollisionNormal;
	CKFVec3					m_vCollisionPos;
	float					m_fPenetration;
	CKFMtx44				m_mtxToWorld;
};

//--------------------------------------------------------------------------------
//  �R���W�������
//--------------------------------------------------------------------------------
class CCollisionInfo
{
public:
	CCollisionInfo()
		: m_pRigidBodyOne(nullptr)
		, m_pRigidBodyTwo(nullptr) 
	{
		m_listCollision.clear();
	}
	~CCollisionInfo() 
	{
		m_listCollision.clear();
	}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	list<CCollision*>		m_listCollision;
};

//--------------------------------------------------------------------------------
//  �������Z�V�X�e��
//--------------------------------------------------------------------------------
class CKFPhysicsSystem
{
public:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	CKFPhysicsSystem() { m_listCollision.clear(); }
	~CKFPhysicsSystem() {}

	bool	Init(void);
	void	Uninit(void);
	void	Release(void);
	void	Update(void);

	void	RegistryCollision(CCollision& collision);

private:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	void	resolve(CCollision& collision);
	void	resolveVelocity(CCollision& collision);
	void	resolveInterpenetration(CCollision& collision);
	float	calculateSeparatingVelocity(CCollision& collision);
	void	calculateCollisionBasis(CCollision& collision);

	//--------------------------------------------------------------------------------
	//  �ϐ��錾
	//--------------------------------------------------------------------------------
	list<CCollision> m_listCollision;
};
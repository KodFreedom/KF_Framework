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
//  �R���W�������
//--------------------------------------------------------------------------------
class CCollisionInfo
{
public:
	CCollisionInfo()
		: m_pRigidBodyOne(nullptr)
		, m_pRigidBodyTwo(nullptr)
		, m_vContactNormal(CKFVec3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollisionInfo() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	CKFVec3					m_vContactNormal;
	float					m_fPenetration;
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
	CKFPhysicsSystem() { m_listCollisionInfo.clear(); }
	~CKFPhysicsSystem() {}

	bool	Init(void);
	void	Uninit(void);
	void	Release(void);
	void	Update(void);

	void	RegistryContact(CCollisionInfo& collisionInfo);

private:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	void	Resolve(CCollisionInfo& collisionInfo);
	void	ResolveVelocity(CCollisionInfo& collisionInfo);
	void	ResolveInterpenetration(CCollisionInfo& collisionInfo);
	float	CalculateSeparatingVelocity(CCollisionInfo& collisionInfo);

	//--------------------------------------------------------------------------------
	//  �ϐ��錾
	//--------------------------------------------------------------------------------
	list<CCollisionInfo> m_listCollisionInfo;
};
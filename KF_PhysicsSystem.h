//--------------------------------------------------------------------------------
//	物理演算システム
//　KF_PhysicsSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コリジョン
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
//  コリジョン情報
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
//  物理演算システム
//--------------------------------------------------------------------------------
class CKFPhysicsSystem
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
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
	//  関数宣言
	//--------------------------------------------------------------------------------
	void	resolve(CCollision& collision);
	void	resolveVelocity(CCollision& collision);
	void	resolveInterpenetration(CCollision& collision);
	float	calculateSeparatingVelocity(CCollision& collision);
	void	calculateCollisionBasis(CCollision& collision);

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	list<CCollision> m_listCollision;
};
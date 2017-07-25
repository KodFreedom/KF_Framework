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
//  コリジョン情報
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
//  物理演算システム
//--------------------------------------------------------------------------------
class CKFPhysicsSystem
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
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
	//  関数宣言
	//--------------------------------------------------------------------------------
	void	Resolve(CCollisionInfo& collisionInfo);
	void	ResolveVelocity(CCollisionInfo& collisionInfo);
	void	ResolveInterpenetration(CCollisionInfo& collisionInfo);
	float	CalculateSeparatingVelocity(CCollisionInfo& collisionInfo);

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	list<CCollisionInfo> m_listCollisionInfo;
};
//--------------------------------------------------------------------------------
//
//　3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "rigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D物理コンポネントクラス
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent : public CRigidbodyComponent
{
public:
	C3DRigidbodyComponent(CGameObject* const pGameObj);
	~C3DRigidbodyComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Update(void) override;

	//Get関数
	CKFVec3	GetVelocity(void) const { return m_vVelocity; }
	bool	IsOnGround(void) const { return m_bOnGround; }

	//Set関数
	void	MovePos(const CKFVec3& vMovement) { m_vMovement = vMovement; }
	void	SetMass(const float& fMass);
	void	SetVelocity(const CKFVec3& vVelocity) { m_vVelocity = vVelocity; }
	void	SetOnGround(const bool& bValue) { m_bOnGround = bValue; }
	void	AddForce(const CKFVec3& vForce) { m_vForceAccum += vForce; }

private:
	float	m_fMass;		//質量
	float	m_fInverseMass;	//質量の逆数
	float	m_fDrag;		//抵抗係数
	CKFVec3	m_vGravity;		//重力
	CKFVec3	m_vMovement;	//移動量
	CKFVec3	m_vVelocity;	//速度
	CKFVec3	m_vForceAccum;	//合わせた作用力
	CKFVec3	m_vAcceleration;//加速度
	bool	m_bOnGround;	//着陸フラッグ
};
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
class CKFPhysicsSystem;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D物理コンポネントクラス
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

	//Get関数
	Vector3	GetVelocity(void) const { return m_vVelocity; }

	//Set関数
	void	SetDrag(const float& fDrag) { m_fDrag = fDrag; }
	void	MovePos(const Vector3& vMovement) { m_vMovement += vMovement; }
	void	SetMass(const float& fMass);
	void	SetVelocity(const Vector3& vVelocity) { m_vVelocity = vVelocity; }
	void	AddForce(const Vector3& vForce) { m_vForceAccum += vForce; }
	//void	SetInertiaTensor(CColliderComponent* pCollider);

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	enum AXIS
	{
		X = 0x01,
		Y = 0x02,
		Z = 0x04,
		XYZ = 0x07
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	//void		calculateInertiaTensorWorld(Matrix44& mtxIitWorld);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	float		m_fMass;					//質量
	float		m_fInverseMass;				//質量の逆数
	float		m_fDrag;					//抵抗係数(空気抵抗)
	//float		AngularDrag;				//回転抵抗係数
	float		m_fFriction;				//摩擦係数
	float		Bounciness;				//跳ね返り係数
	float		GravityCoefficient;		//重力係数
	Vector3		m_vMovement;				//移動量
	Vector3		m_vVelocity;				//速度
	Vector3		m_vAcceleration;			//加速度
	//Vector3		m_vAngularVelocity;			//回転速度
	Vector3		m_vForceAccum;				//合わせた作用力
	//Vector3		m_vTorqueAccum;				//回転力
	//Matrix44	m_mtxInertisTensor;			//慣性テンソルの行列
	//BYTE		m_bRotLock;					//回転制限のフラグ
};
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
#include "rigidbody.h"

//--------------------------------------------------------------------------------
//  クラス宣言
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

	//Get関数
	auto	GetVelocity(void) const { return velocity; }

	//Set関数
	void	SetDrag(const float& value) { drag = value; }
	void	SetMass(const float& value);
	void	SetVelocity(const Vector3& value) { velocity = value; }
	void	AddForce(const Vector3& force) { forceAccum += force; }
	void	Move(const Vector3& value) { movement += value; }
	//void	SetInertiaTensor(Collider* pCollider);

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	//enum AXIS
	//{
	//	X = 0x01,
	//	Y = 0x02,
	//	Z = 0x04,
	//	XYZ = 0x07
	//};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	//void		calculateInertiaTensorWorld(Matrix44& mtxIitWorld);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	float		mass;					//質量
	float		inverseMass;				//質量の逆数
	float		drag;					//抵抗係数(空気抵抗)
	//float		AngularDrag;				//回転抵抗係数
	float		friction;				//摩擦係数
	float		bounciness;				//跳ね返り係数
	float		gravityCoefficient;		//重力係数
	Vector3		movement;				//移動量
	Vector3		velocity;				//速度
	Vector3		acceleration;			//加速度
	//Vector3	m_vAngularVelocity;			//回転速度
	Vector3		forceAccum;				//合わせた作用力
	//Vector3	m_vTorqueAccum;				//回転力
	//Matrix44	m_mtxInertisTensor;			//慣性テンソルの行列
	//BYTE		m_bRotLock;					//回転制限のフラグ
};
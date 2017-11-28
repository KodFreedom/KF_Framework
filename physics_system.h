//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@physics_system.h
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Collision;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PhysicsSystem
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const Vector3 kGravity;

	//--------------------------------------------------------------------------------
	//  ��������
	//  return : CollisionSystem*
	//--------------------------------------------------------------------------------
	static PhysicsSystem* Create(void)
	{
		auto instance = MY_NEW PhysicsSystem;
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  �N���A����
	//--------------------------------------------------------------------------------
	void Clear(void);

	//--------------------------------------------------------------------------------
	//  �o�^����
	//--------------------------------------------------------------------------------
	void Register(Collision* collision) { collisions_.push_back(collision); }

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	PhysicsSystem() {}
	PhysicsSystem(const PhysicsSystem& value) {}
	PhysicsSystem& operator=(const PhysicsSystem& value) {}
	~PhysicsSystem() {}

	//--------------------------------------------------------------------------------
	//  �Փˉ�������
	//--------------------------------------------------------------------------------
	void Resolve(Collision& collision);

	//--------------------------------------------------------------------------------
	//  �Փˑ��x��������
	//--------------------------------------------------------------------------------
	void ResolveVelocity(Collision& collision);

	//--------------------------------------------------------------------------------
	//  �Փˈʒu��������
	//--------------------------------------------------------------------------------
	void ResolveInterpenetration(Collision& collision);

	//--------------------------------------------------------------------------------
	//  �������x�v�Z����
	//--------------------------------------------------------------------------------
	float CalculateSeparatingVelocity(Collision& collision);
	//void	calculateCollisionBasis(Collision& collision);

	//--------------------------------------------------------------------------------
	//  �ϐ��錾
	//--------------------------------------------------------------------------------
	list<Collision*> collisions_;
};
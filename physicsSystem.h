//--------------------------------------------------------------------------------
//	�������Z�V�X�e��
//�@KF_PhysicsSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
#pragma once

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
	static const Vector3 Gravity;

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new PhysicsSystem;
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void Update(void);
	void Clear(void);
	void Register(Collision* collision) { collisions.push_back(collision); }

private:
	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	PhysicsSystem() { collisions.clear(); }
	~PhysicsSystem() {}
	void	uninit(void) { Clear(); }
	void	resolve(Collision& collision);
	void	resolveVelocity(Collision& collision);
	void	resolveInterpenetration(Collision& collision);
	float	calculateSeparatingVelocity(Collision& collision);
	//void	calculateCollisionBasis(Collision& collision);

	//--------------------------------------------------------------------------------
	//  �ϐ��錾
	//--------------------------------------------------------------------------------
	list<Collision*>		collisions;
	static PhysicsSystem*	instance;
};
//--------------------------------------------------------------------------------
//	物理演算システム
//　physics_system.h
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Collision;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PhysicsSystem
{
public:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const Vector3 kGravity;

	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : CollisionSystem*
	//--------------------------------------------------------------------------------
	static PhysicsSystem* Create(void)
	{
		auto instance = MY_NEW PhysicsSystem;
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  クリア処理
	//--------------------------------------------------------------------------------
	void Clear(void);

	//--------------------------------------------------------------------------------
	//  登録処理
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
	//  衝突解決処理
	//--------------------------------------------------------------------------------
	void Resolve(Collision& collision);

	//--------------------------------------------------------------------------------
	//  衝突速度解決処理
	//--------------------------------------------------------------------------------
	void ResolveVelocity(Collision& collision);

	//--------------------------------------------------------------------------------
	//  衝突位置解決処理
	//--------------------------------------------------------------------------------
	void ResolveInterpenetration(Collision& collision);

	//--------------------------------------------------------------------------------
	//  分離速度計算処理
	//--------------------------------------------------------------------------------
	float CalculateSeparatingVelocity(Collision& collision);
	//void	calculateCollisionBasis(Collision& collision);

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	list<Collision*> collisions_;
};
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
	static const Vector3 Gravity;

	//--------------------------------------------------------------------------------
	//  関数宣言
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
	void Register(Collision* collision);

private:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	PhysicsSystem() { collisions.clear(); }
	~PhysicsSystem() {}
	void	uninit(void);
	void	resolve(Collision& collision);
	void	resolveVelocity(Collision& collision);
	void	resolveInterpenetration(Collision& collision);
	float	calculateSeparatingVelocity(Collision& collision);
	void	calculateCollisionBasis(Collision& collision);

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	list<Collision*>		collisions;
	static PhysicsSystem*	instance;
};
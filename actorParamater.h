//--------------------------------------------------------------------------------
//	パラメータークラス
//　actorParamater.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ActorParamater
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ActorParamater()
		: level(0)
		, maxLife(0.0f)
		, currentLife(0.0f)
		, attack(0.0f)
		, defence(0.0f)
		, moveSpeed(0.0f)
		, jumpSpeed(0.0f)
		, minTurnSpeed(0.0f)
		, maxTurnSpeed(0.0f)
	{}
	~ActorParamater() {}

	void SetLevel(const int& value) { level = value; }
	void SetMaxLife(const float& value) { maxLife = value; }
	void SetCurrentLife(const float& value) { currentLife = value; }
	void SetAttack(const float& value) { attack = value; }
	void SetDefence(const float& value) { defence = value; }
	void SetMoveSpeed(const float& value) { moveSpeed = value; }
	void SetJumpSpeed(const float& value) { jumpSpeed = value; }
	void SetMinTurnSpeed(const float& value) { minTurnSpeed = value; }
	void SetMaxTurnSpeed(const float& value) { maxTurnSpeed = value; }
	
	auto GetMaxLife(void) const { return maxLife; }
	auto GetCurrentLife(void) const { return currentLife; }
	auto GetMoveSpeed(void) const { return moveSpeed; }
	auto GetJumpSpeed(void) const { return jumpSpeed; }
	auto GetMinTurnSpeed(void) const { return minTurnSpeed; }
	auto GetMaxTurnSpeed(void) const { return maxTurnSpeed; }

private:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int		level;
	float	maxLife;
	float	currentLife;
	float	attack;
	float	defence;
	float	moveSpeed;
	float	jumpSpeed;
	float	minTurnSpeed;
	float	maxTurnSpeed;
};
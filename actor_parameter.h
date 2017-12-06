//--------------------------------------------------------------------------------
//	パラメータークラス
//　actor_parameter.h
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ActorParameter
{
	friend class DebugObserver;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ActorParameter()
		: level_(1)
		, max_life_(1.0f)
		, current_life_(1.0f)
		, attack_(1.0f)
		, defence_(1.0f)
		, move_speed_(1.0f)
		, jump_speed_(1.0f)
		, min_turn_speed_(0.0f)
		, max_turn_speed_(kPi)
		, movement_multiplier_(1.0f)
		, ground_check_distance_(0.0f)
	{}
	~ActorParameter() {}

	void SetLevel(const int& value) { level_ = value; }
	void SetMaxLife(const float& value) { max_life_ = value; }
	void SetCurrentLife(const float& value) { current_life_ = value; }
	void SetAttack(const float& value) { attack_ = value; }
	void SetDefence(const float& value) { defence_ = value; }
	void SetMoveSpeed(const float& value) { move_speed_ = value; }
	void SetJumpSpeed(const float& value) { jump_speed_ = value; }
	void SetMinTurnSpeed(const float& value) { min_turn_speed_ = value; }
	void SetMaxTurnSpeed(const float& value) { max_turn_speed_ = value; }
	void SetMovementMultiplier(const float& value) { movement_multiplier_ = value; }
	void SetGroundCheckDistance(const float& value) { ground_check_distance_ = value; }
	
	auto GetMaxLife(void) const { return max_life_; }
	auto GetCurrentLife(void) const { return current_life_; }
	auto GetMoveSpeed(void) const { return move_speed_; }
	auto GetJumpSpeed(void) const { return jump_speed_; }
	auto GetMinTurnSpeed(void) const { return min_turn_speed_; }
	auto GetMaxTurnSpeed(void) const { return max_turn_speed_; }
	const float& GetMovementMultiplier(void) const { return movement_multiplier_; }
	const float& GetGroundCheckDistance(void) const { return ground_check_distance_; }

private:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int   level_;
	float max_life_;
	float current_life_;
	float attack_;
	float defence_;
	float move_speed_;
	float jump_speed_;
	float min_turn_speed_;
	float max_turn_speed_;
	float movement_multiplier_;
	float ground_check_distance_;
};
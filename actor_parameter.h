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
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ActorParameter()
		: level_(0)
		, max_life_(0.0f)
		, current_life_(0.0f)
		, attack_(0.0f)
		, defence_(0.0f)
		, move_speed_(0.0f)
		, jump_speed_(0.0f)
		, min_turn_speed_(0.0f)
		, max_turn_speed_(0.0f)
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
	
	auto GetMaxLife(void) const { return max_life_; }
	auto GetCurrentLife(void) const { return current_life_; }
	auto GetMoveSpeed(void) const { return move_speed_; }
	auto GetJumpSpeed(void) const { return jump_speed_; }
	auto GetMinTurnSpeed(void) const { return min_turn_speed_; }
	auto GetMaxTurnSpeed(void) const { return max_turn_speed_; }

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
};
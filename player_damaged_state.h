//--------------------------------------------------------------------------------
//	ダメージ受けたステート
//　player_damaged_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerDamagedState : public PlayerState
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	PlayerDamagedState() : PlayerState(L"PlayerDamagedState"), time_counter_(kWaitTime) {}
	~PlayerDamagedState() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	void Init(ActorController& actor) override;

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(ActorController& actor) override;

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(ActorController& actor) override;

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	void LateUpdate(ActorController& actor) override {}

	//--------------------------------------------------------------------------------
	//  コライダートリガーの時呼ばれる
	//--------------------------------------------------------------------------------
	void OnTrigger(ActorController& actor, Collider& self, Collider& other) {}

	//--------------------------------------------------------------------------------
	//  コライダー衝突の時呼ばれる
	//--------------------------------------------------------------------------------
	void OnCollision(ActorController& actor, CollisionInfo& info) {}

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kWaitTime = 3.0f;
	static constexpr float kDamagedMovementMultiplier = 0.0f;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	float time_counter_;
};
//--------------------------------------------------------------------------------
//	攻撃ステート
//　player_attack_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerAttackState : public PlayerState
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	PlayerAttackState() : PlayerState(L"PlayerAttackState") {}
	~PlayerAttackState() {}

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
	void OnTrigger(ActorController& actor, Collider& self, Collider& other);

	//--------------------------------------------------------------------------------
	//  コライダー衝突の時呼ばれる
	//--------------------------------------------------------------------------------
	void OnCollision(ActorController& actor, CollisionInfo& info);

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kMovementMultiplier = 0.0f;
};
//--------------------------------------------------------------------------------
//	mutant歩くステート
//　player_mutant_walk_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerMutantWalkState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerMutantWalkState() : PlayerState(L"PlayerMutantWalkState") {}
    ~PlayerMutantWalkState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(PlayerController& actor) override {}

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(PlayerController& actor, Collider& self, Collider& other);

    //--------------------------------------------------------------------------------
    //  コライダー衝突の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnCollision(PlayerController& actor, CollisionInfo& info);

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kGroundCheckDistance = 0.5f;
    static constexpr float kMovementMultiplier = 1.0f;
};
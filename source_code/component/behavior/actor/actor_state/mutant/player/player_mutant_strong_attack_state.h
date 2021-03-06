//--------------------------------------------------------------------------------
//  mutant重攻撃ステート
//  player_mutant_strong_attack_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../../player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerMutantStrongAttackState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerMutantStrongAttackState() : PlayerState(L"PlayerMutantStrongAttackState") {}
    ~PlayerMutantStrongAttackState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(PlayerController& player) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(PlayerController& player) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(PlayerController& player) override;

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(PlayerController& player, Collider& self, Collider& other) override;

    //--------------------------------------------------------------------------------
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    void OnDamaged(PlayerController& player, const float& damage) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.0f;
    static constexpr int   kBeginAttackFrame = 46;
    static constexpr int   kEndAttackFrame = 100;
};
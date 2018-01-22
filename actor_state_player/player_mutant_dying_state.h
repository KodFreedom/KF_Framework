//--------------------------------------------------------------------------------
//  mutant死亡ステート
//  player_mutant_dying_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerMutantDyingState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerMutantDyingState() : PlayerState(L"PlayerMutantDyingState") {}
    ~PlayerMutantDyingState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(PlayerController& actor) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.0f;
};
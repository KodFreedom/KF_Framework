//--------------------------------------------------------------------------------
//  knight歩くステート
//  player_knight_walk_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../../player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerKnightWalkState final : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerKnightWalkState() : PlayerState(L"PlayerKnightWalkState") {}
    ~PlayerKnightWalkState() {}

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
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    void OnDamaged(PlayerController& player, const float& damage) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kGroundCheckDistance = 0.5f;
    static constexpr float kMovementMultiplier = 1.0f;
};
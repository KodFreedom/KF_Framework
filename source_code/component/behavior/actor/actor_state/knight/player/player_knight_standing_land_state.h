//--------------------------------------------------------------------------------
//  knight着地ステート
//  player_knight_standing_land_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerKnightStandingLandState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerKnightStandingLandState() : PlayerState(L"PlayerKnightStandingLandState") {}
    ~PlayerKnightStandingLandState() {}

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

    //--------------------------------------------------------------------------------
    //  モーション終了の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnAnimationOver(PlayerController& player) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.0f;
};
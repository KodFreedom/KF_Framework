//--------------------------------------------------------------------------------
//  knight軽攻撃ステップ1ステート
//　player_knight_light_attack_step1_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../../player_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerKnightLightAttackStep1State final : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerKnightLightAttackStep1State() : PlayerState(L"PlayerKnightLightAttackStep1State") {}
    ~PlayerKnightLightAttackStep1State() {}

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

    //--------------------------------------------------------------------------------
    //  モーション終了の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnAnimationOver(PlayerController& player) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.25f;  
    static constexpr int kBeginAttackFrame = 35;
    static constexpr int kEndAttackFrame = 48;
    static constexpr int kBeginStep2Frame = 48;
};
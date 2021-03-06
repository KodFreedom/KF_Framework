//--------------------------------------------------------------------------------
//  zombieダメージ受けたステート
//　enemy_zombie_damaged_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../../enemy_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class EnemyZombieDamagedState : public EnemyState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    EnemyZombieDamagedState() : EnemyState(L"EnemyZombieDamagedState") {}
    ~EnemyZombieDamagedState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(EnemyController& enemy) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(EnemyController& enemy) override;

    //--------------------------------------------------------------------------------
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    void OnDamaged(EnemyController& enemy, const float& damage) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kInvincibleTime = 0.3f;

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    float time_counter_ = 0.0f;
};
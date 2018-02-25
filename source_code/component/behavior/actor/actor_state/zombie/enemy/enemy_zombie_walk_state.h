//--------------------------------------------------------------------------------
//  zombie歩くステート
//　enemy_zombie_walk_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../../enemy_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class EnemyZombieWalkState : public EnemyState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    EnemyZombieWalkState() : EnemyState(L"EnemyZombieWalkState") {}
    ~EnemyZombieWalkState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(EnemyController& enemy) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(EnemyController& enemy) override;

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(EnemyController& enemy, Collider& self, Collider& other) override;

    //--------------------------------------------------------------------------------
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    void OnDamaged(EnemyController& enemy, const float& damage) override;

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kGroundCheckDistance = 0.5f;
    static constexpr float kMovementMultiplier = 0.1f;
    static constexpr float kArriveDistance = 1.0f;
};
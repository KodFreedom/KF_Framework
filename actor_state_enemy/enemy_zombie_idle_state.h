//--------------------------------------------------------------------------------
//  zombie待機ステート
//　enemy_zombie_idel_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "enemy_state.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class EnemyZombieIdleState : public EnemyState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    EnemyZombieIdleState() : EnemyState(L"EnemyZombieIdleState"), time_counter_(0.0f) {}
    ~EnemyZombieIdleState() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(EnemyController& actor) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(EnemyController& actor) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(EnemyController& actor) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(EnemyController& actor) override {}

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(EnemyController& actor, Collider& self, Collider& other);

    //--------------------------------------------------------------------------------
    //  コライダー衝突の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnCollision(EnemyController& actor, CollisionInfo& info);

private:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kGroundCheckDistance = 0.5f;
    static constexpr float kWaitTime = 20.0f;

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    float time_counter_;
};
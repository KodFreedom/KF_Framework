//--------------------------------------------------------------------------------
//  zombie追跡ステート
//　enemy_zombie_follow_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_attack_state.h"
#include "enemy_zombie_walk_state.h"
#include "enemy_zombie_follow_state.h"
#include "enemy_zombie_damaged_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::Init(EnemyController& enemy)
{
    auto& parameter = enemy.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::Uninit(EnemyController& enemy)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::Update(EnemyController& enemy)
{
    // プレイヤーに追跡
    const Vector3& player_position = enemy.GetTarget()->GetTransform()->GetPosition();
    const Vector3& my_position = enemy.GetGameObject().GetTransform()->GetPosition();
    Vector3& me_to_player = player_position - my_position;
    float square_distance = me_to_player.SquareMagnitude();
    float warning_range = enemy.GetWarningRange();
    
    if (square_distance > warning_range * warning_range)
    {// 範囲外になるので追跡中止
        enemy.SetTarget(nullptr);
        enemy.Change(MY_NEW EnemyZombieWalkState);
        return;
    }

    if (square_distance <= kAttackRange * kAttackRange)
    {// 攻撃範囲内になるので攻撃する
        enemy.Change(MY_NEW EnemyZombieAttackState);
        return;
    }

    me_to_player = Vector3::Scale(me_to_player, Vector3(1.0f, 0.0f, 1.0f)).Normalized();
    enemy.SetMovement(me_to_player);
    enemy.CheckGrounded();
    enemy.Move();
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{

}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieFollowState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

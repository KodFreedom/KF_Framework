//--------------------------------------------------------------------------------
//  zombie待機ステート
//　enemy_zombie_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "../enemy_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Init(EnemyController& enemy)
{
    enemy.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    enemy.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Uninit(EnemyController& enemy)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Update(EnemyController& enemy)
{
    enemy.CheckGrounded();
    enemy.Move();
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnTrigger(EnemyController& enemy, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"detector"))
    {
        if (other.GetGameObject().GetTag()._Equal(L"Player"))
        {
            enemy.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnCollision(EnemyController& enemy, CollisionInfo& info)
{

}

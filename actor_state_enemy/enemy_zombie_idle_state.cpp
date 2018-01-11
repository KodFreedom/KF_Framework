//--------------------------------------------------------------------------------
//  zombie待機ステート
//　enemy_zombie_idel_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_zombie_idle_state.h"
#include "../actor_controller.h"
#include "../animator.h"
#include "../collider.h"
#include "../game_object.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Init(ActorController& actor)
{
    actor.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    actor.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Uninit(ActorController& actor)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::Update(ActorController& actor)
{
    actor.CheckGrounded();
    actor.Move();
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnTrigger(ActorController& actor, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"detector"))
    {
        if (other.GetGameObject().GetTag()._Equal(L"Player"))
        {
            actor.SetTarget(&other.GetGameObject());
        }
    }
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyZombieIdelState::OnCollision(ActorController& actor, CollisionInfo& info)
{

}

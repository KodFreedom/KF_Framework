//--------------------------------------------------------------------------------
//　actor_observer.cpp
//  アクター（プレイヤー、エネミー）の観察者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "actor_observer.h"
#include "../component/behavior/actor/player_controller.h"
#include "../component/behavior/actor/enemy_controller.h"
#include "../game_object/game_object.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Enemy登録処理
//--------------------------------------------------------------------------------
void ActorObserver::Register(EnemyController* enemy)
{
    assert(enemy);
    enemys_.push_back(enemy);
}

//--------------------------------------------------------------------------------
//  Enemy削除処理
//--------------------------------------------------------------------------------
void ActorObserver::Deregister(EnemyController* enemy)
{
    assert(enemy);
    enemys_.remove(enemy);
}
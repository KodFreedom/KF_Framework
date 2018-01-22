//--------------------------------------------------------------------------------
//　actor_observer.h
//  アクター（プレイヤー、エネミー）の観察者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class PlayerController;
class EnemyController;

//--------------------------------------------------------------------------------
//  アクターの観察者
//--------------------------------------------------------------------------------
class ActorObserver
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : ActorObserver*
    //--------------------------------------------------------------------------------
    static ActorObserver* Create(void)
    {
        auto instance = MY_NEW ActorObserver;
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void)
    {
        Clear();
        MY_DELETE this;
    }

    //--------------------------------------------------------------------------------
    //  クリア処理
    //--------------------------------------------------------------------------------
    void Clear(void)
    {
        player_ = nullptr;
        enemys_.clear();
    }

    //--------------------------------------------------------------------------------
    //  Player処理
    //--------------------------------------------------------------------------------
    void Register(PlayerController* player) { player_ = player; }
    void Deregister(PlayerController* player)
    {
        assert(player == player_);
        player_ = nullptr;
    }
    auto GetPlayer(void) const { return player_; }

    //--------------------------------------------------------------------------------
    //  Enemy処理
    //--------------------------------------------------------------------------------
    void  Register(EnemyController* enemy);
    void  Deregister(EnemyController* enemy);
    auto& GetEnemys(void) const { return enemys_; }

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    ActorObserver() {}
    ActorObserver(const ActorObserver& value) {}
    ActorObserver& operator=(const ActorObserver& value) {}
    ~ActorObserver() {}

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    PlayerController* player_;
    list<EnemyController*> enemys_;
};
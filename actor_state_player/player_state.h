//--------------------------------------------------------------------------------
//	プレイヤーステートマシン
//　player_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../actor_state.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class PlayerController;
class Collider;
class CollisionInfo;

//--------------------------------------------------------------------------------
//  プレイヤーステートの基底クラス
//--------------------------------------------------------------------------------
class PlayerState : public ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	PlayerState(const String& name) : ActorState(name) {}
	~PlayerState() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	virtual void Init(PlayerController& actor) {}

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	virtual void Uninit(PlayerController& actor) {}

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	virtual void Update(PlayerController& actor)
	{
		UpdateInput(actor);
	}

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	virtual void LateUpdate(PlayerController& actor) {}

	//--------------------------------------------------------------------------------
	//  コライダートリガーの時呼ばれる
	//--------------------------------------------------------------------------------
	virtual void OnTrigger(PlayerController& actor, Collider& self, Collider& other) {}

	//--------------------------------------------------------------------------------
	//  コライダー衝突の時呼ばれる
	//--------------------------------------------------------------------------------
	virtual void OnCollision(PlayerController& actor, CollisionInfo& info) {}

    //--------------------------------------------------------------------------------
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    virtual void OnDamaged(PlayerController& actor) {}

private:
	//--------------------------------------------------------------------------------
	//  操作更新
	//--------------------------------------------------------------------------------
	void UpdateInput(PlayerController& actor);
};


//--------------------------------------------------------------------------------
//  ヌルプレイヤーステート
//--------------------------------------------------------------------------------
class NullPlayerState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    NullPlayerState() : PlayerState(L"NullPlayerState") {}
    ~NullPlayerState() {}
};
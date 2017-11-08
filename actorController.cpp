//--------------------------------------------------------------------------------
//	生き物コントローラ
//　ActorController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "actorController.h"
#include "gameObjectActor.h"
#include "actorState.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObjectActor* const owner, const string& name, Rigidbody3D& rigidbody)
	: Behavior(owner, name), currentState(nullptr), rigidbody(rigidbody)
{
	animator = owner->GetAnimator();
}

//--------------------------------------------------------------------------------
//	関数名：Change
//  関数説明：ステートの切り替え
//	引数：	state：最新のステート
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorController::Change(ActorState* state)
{
	SAFE_DELETE(currentState);
	currentState = state;
	currentState->Init(*this);
}
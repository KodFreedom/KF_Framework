//--------------------------------------------------------------------------------
//	生き物コントローラ
//　ActorController.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "actor_controller.h"
#include "game_object.h"
#include "actor_state.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObject& owner, const String& name, Rigidbody3D& rigidbody, Animator& animator)
	: Behavior(owner, name), state_(nullptr), rigidbody_(rigidbody), animator_(animator)
{}

//--------------------------------------------------------------------------------
//	関数名：Change
//  関数説明：ステートの切り替え
//	引数：	state：最新のステート
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorController::Change(ActorState* state)
{
	SAFE_DELETE(state_);
	state_ = state;
	state_->Init(*this);
}
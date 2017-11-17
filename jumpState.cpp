//--------------------------------------------------------------------------------
//	跳ぶステート
//　jumpState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "jumpState.h"
#include "landState.h"
#include "actorController.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float JumpState::airborneGroundCheckDistance = 0.1f;

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
JumpState::JumpState()
	: ActorState("Jump", 0.5f) {}

//--------------------------------------------------------------------------------
//	関数名：Init
//  関数説明：初期化関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void JumpState::Init(ActorController& actor)
{
	groundCheckDistance = 0.0f;
	auto animator = actor.GetAnimator();
	animator->SetGrounded(false);
	animator->SetJump(true);
	animator->SetMovement(0.0f);
}

//--------------------------------------------------------------------------------
//	関数名：Act
//  関数説明：行動関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void JumpState::Act(ActorController& actor)
{
	checkGrounded(actor);
	move(actor);
	if (currentGroundState.IsGrounded)
	{
		actor.Change(new LandState);
		return;
	}
	groundCheckDistance = airborneGroundCheckDistance;
}
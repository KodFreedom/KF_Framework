//--------------------------------------------------------------------------------
//	着地ステート
//　LandState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "landState.h"
#include "walkState.h"
#include "neutralState.h"
#include "jumpState.h"
#include "actorController.h"
#include "animator.h"

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
LandState::LandState()
	: ActorState("Land") {}

//--------------------------------------------------------------------------------
//	関数名：Init
//  関数説明：初期化関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void LandState::Init(ActorController& actor)
{
	auto animator = actor.GetAnimator();
	animator->SetJump(false);
	animator->SetGrounded(true);
}

//--------------------------------------------------------------------------------
//	関数名：Act
//  関数説明：行動関数
//	引数：	actor：アクターコントローラ
//	戻り値：なし
//--------------------------------------------------------------------------------
void LandState::Act(ActorController& actor)
{
	if (actor.GetMovement().SquareMagnitude() > 0.0f)
	{
		actor.Change(new WalkState);
		return;
	}

	if (actor.GetIsJump())
	{
		jump(actor);
		actor.Change(new JumpState);
		return;
	}

	if (!actor.GetAnimator()->GetCurrentAnimationName()._Equal("Landing"))
	{
		actor.Change(new NeutralState);
		return;
	}
}
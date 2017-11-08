//--------------------------------------------------------------------------------
//	跳ぶステート
//　JumpState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorState.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class JumpState : public ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	JumpState();
	~JumpState() {}

	void Init(ActorController& actor) override;
	void Act(ActorController& actor) override;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static const float airborneGroundCheckDistance;
};
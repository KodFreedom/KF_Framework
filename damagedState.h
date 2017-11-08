//--------------------------------------------------------------------------------
//	ダメージ受けたステート
//　DamagedState.h
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
class DamagedState : public ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	DamagedState();
	~DamagedState() {}

	void Init(ActorController& actor) override;
	void Act(ActorController& actor) override;
	void OnDamaged(ActorController& actor, const float& damage) override {}
};
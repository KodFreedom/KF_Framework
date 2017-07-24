//--------------------------------------------------------------------------------
//	Player移動状態
//　playerMoveStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "status.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CPlayerMoveStatus : public CStatus
{
public:
	CPlayerMoveStatus() : CStatus() {}
	~CPlayerMoveStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;
};
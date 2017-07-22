//--------------------------------------------------------------------------------
//	プレイやノーマルステータス
//　playerNormalStatus.h
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
class CPlayerNormalStatus : public CStatus
{
public:
	CPlayerNormalStatus() {}
	~CPlayerNormalStatus() override {}

	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;
};
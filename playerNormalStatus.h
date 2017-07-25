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
	CPlayerNormalStatus() : CStatus() {}
	~CPlayerNormalStatus() override {}
	void Update(CPlayerBehaviorComponent& player) override;
	void LateUpdate(CPlayerBehaviorComponent& player) override;
};
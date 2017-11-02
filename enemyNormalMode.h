//--------------------------------------------------------------------------------
//	エネミーノーマルモード
//　enemyNormalMode.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "AIMode.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CEnemyNormalMode : public CAIMode
{
public:
	CEnemyNormalMode() : CAIMode(ANormal) {}
	~CEnemyNormalMode() override {}
	void Update(CEnemyBehaviorComponent& enemy) override;
};
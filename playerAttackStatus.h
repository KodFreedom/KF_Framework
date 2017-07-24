//--------------------------------------------------------------------------------
//	プレイヤー攻撃ステータス
//　playerAttackStatus.h
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
class CPlayerAttackStatus : public CStatus
{
public:
	CPlayerAttackStatus() : CStatus(), m_bFirst(true) {}
	~CPlayerAttackStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;

private:
	bool m_bFirst;
};
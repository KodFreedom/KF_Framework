//--------------------------------------------------------------------------------
//	プレイヤージャンプステータス
//　playerJumpStatus.h
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
class CPlayerJumpStatus : public CStatus
{
public:
	CPlayerJumpStatus() : CStatus(), m_bFirst(true) {}
	~CPlayerJumpStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;

private:
	bool m_bFirst;
};
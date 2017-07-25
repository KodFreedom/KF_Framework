//--------------------------------------------------------------------------------
//	Player�ړ����
//�@playerMoveStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "status.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CPlayerMoveStatus : public CStatus
{
public:
	CPlayerMoveStatus() : CStatus() {}
	~CPlayerMoveStatus() override {}
	void Update(CPlayerBehaviorComponent& player) override;
	void LateUpdate(CPlayerBehaviorComponent& player) override;
};
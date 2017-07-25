//--------------------------------------------------------------------------------
//	�v���C��m�[�}���X�e�[�^�X
//�@playerNormalStatus.h
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
class CPlayerNormalStatus : public CStatus
{
public:
	CPlayerNormalStatus() : CStatus() {}
	~CPlayerNormalStatus() override {}
	void Update(CPlayerBehaviorComponent& player) override;
	void LateUpdate(CPlayerBehaviorComponent& player) override;
};
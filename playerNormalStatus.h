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
	CPlayerNormalStatus() {}
	~CPlayerNormalStatus() override {}

	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;
};
//--------------------------------------------------------------------------------
//	�X�e�[�^�X�R���|�l���g
//�@statusComponent.h
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
class CPlayerJumpStatus : public CStatus
{
public:
	CPlayerJumpStatus();
	~CPlayerJumpStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;
};
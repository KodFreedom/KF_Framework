//--------------------------------------------------------------------------------
//	�v���C���[�W�����v�X�e�[�^�X
//�@playerJumpStatus.h
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
	CPlayerJumpStatus() : CStatus(), m_bFirst(true) {}
	~CPlayerJumpStatus() override {}
	void Update(CPlayerBehaviorComponent& player) override;
	void LateUpdate(CPlayerBehaviorComponent& player) override;

private:
	bool m_bFirst;
};
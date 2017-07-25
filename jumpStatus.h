//--------------------------------------------------------------------------------
//	�W�����v�X�e�[�^�X
//�@jumpStatus.h
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
class CJumpStatus : public CStatus
{
public:
	CJumpStatus() : CStatus(), m_bFirst(true) {}
	~CJumpStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;

private:
	bool m_bFirst;
};
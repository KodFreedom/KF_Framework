//--------------------------------------------------------------------------------
//	�v���C���[�U���X�e�[�^�X
//�@playerAttackStatus.h
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